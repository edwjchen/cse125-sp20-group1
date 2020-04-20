#include <cstdlib>
#include <deque>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

using namespace boost::asio;
using namespace std;

using ip::tcp;

typedef std::deque<std::string> chat_message_queue;

class chat_client
{
public:
  chat_client(boost::asio::io_service& io_service,
      tcp::endpoint endpoint)
    : io_service_(io_service),
      socket_(io_service)
  {
    socket_.async_connect(endpoint,
        boost::bind(&chat_client::handle_connect, this,
          boost::asio::placeholders::error, endpoint));
  }

  string getMsg()
  {
      // For testing only
      istream buffer(&read_msg_);
      string msg;
      buffer >> msg;
      return msg;
  }
    
  void write(const std::string msg)
  {
    io_service_.post(boost::bind(&chat_client::do_write, this, msg));
  }

  void close()
  {
    io_service_.post(boost::bind(&chat_client::do_close, this));
  }

private:

  void handle_connect(const boost::system::error_code& error,
      tcp::endpoint endpoint)
  {
    if (!error)
    {
      boost::asio::async_read_until(socket_,
          read_msg_, '\n',
          boost::bind(&chat_client::handle_read, this,
            boost::asio::placeholders::error));
    }else{
        std::cout << "Connection refused!" << std::endl;
    }
  }

  void handle_read(const boost::system::error_code& error)
  {
    if (!error)
    {
      boost::asio::async_read_until(socket_,
          read_msg_, '\n',
          boost::bind(&::chat_client::handle_read, this, boost::asio::placeholders::error));
    }
    else
    {
      do_close();
    }
  }

  void do_write(std::string msg)
  {
    bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg+'\n');
    if (!write_in_progress)
    {
      boost::asio::async_write(socket_,
          boost::asio::buffer(write_msgs_.front().data(),
            write_msgs_.front().length()),
          boost::bind(&chat_client::handle_write, this,
            boost::asio::placeholders::error));
    }
  }

  void handle_write(const boost::system::error_code& error)
  {
    if (!error)
    {
      write_msgs_.pop_front();
      if (!write_msgs_.empty())
      {
        boost::asio::async_write(socket_,
            boost::asio::buffer(write_msgs_.front().data(),
              write_msgs_.front().length()),
            boost::bind(&chat_client::handle_write, this,
              boost::asio::placeholders::error));
      }
    }
    else
    {
      do_close();
    }
  }

  void do_close()
  {
    socket_.close();
  }

private:
  boost::asio::io_service& io_service_;
  tcp::socket socket_;
  boost::asio::streambuf read_msg_;
  chat_message_queue write_msgs_;
};

/*
int main(int argc, char* argv[])
{
  try
  {

    boost::asio::io_service io_service;
    tcp::endpoint endpoint(ip::address::from_string("127.0.0.1"),8888);

    chat_client c(io_service, endpoint);

    boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));

    std::string msg;
    while (getline(std::cin, msg))
    {
      c.write(msg);
    }

    c.close();
    t.join();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
 */
