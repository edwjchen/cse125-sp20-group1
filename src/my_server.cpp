#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
using namespace boost::asio;
using ip::tcp;
using std::cout;
using std::endl;
#include <thread>
#include <chrono>
#include <ctime>
#include <string>
#include <deque>
#include <iostream>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <thread>
#include <mutex>
#include <algorithm>
#include <iomanip>
#include <array>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

class Server : public boost::enable_shared_from_this<Server>
{
private:
  boost::asio::io_service& io_service_;
  tcp::acceptor acceptor_;
  int i = 0;

  char info [2];
  char info_buffer[1024];

  void send_info(int id, std::shared_ptr<tcp::socket> socket){
    while(1){
      const std::string msg = "you are " + std::to_string(id) + " info: " + info[0] + ", " + info[1] + "\n";
      // socket->async_read_some(
      //   boost::asio::buffer(info_buffer),
      //   boost::bind(&Server::handle_read, 
      //               shared_from_this(),
      //               boost::asio::placeholders::error,
      //               boost::asio::placeholders::bytes_transferred, id));
      cout << info[0] << ", " << info[1] << endl;
      boost::asio::write( *socket, boost::asio::buffer(msg) );
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
  }

  void read_info(int id, std::shared_ptr<tcp::socket> socket)
  {
    while(1){
      boost::asio::streambuf buf;
      boost::asio::read_until( *socket, buf, "\n" );
      std::string data = boost::asio::buffer_cast<const char*>(buf.data());
      cout << data << endl;
      info[id - 1] = data.at(0);
    }
  }

  void start_accept()
  {
    // socket
    cout << "begin accepting" << endl;
   while (i < 2){
      boost::asio::io_service io_service;
      tcp::socket socket_(io_service);
      std::shared_ptr<tcp::socket> socket_1(new tcp::socket(io_service));
      acceptor_.accept(*socket_1);
      i++;
      cout << "accepted: " << i << endl;
      //update(i, socket_1);
      boost::thread send_thread(&Server::send_info, this, i, socket_1);
      boost::thread read_thread(&Server::read_info, this, i, socket_1);
    }
    while(1){

    }
    cout << "finish accepting" << endl;
  }

public:
  //constructor for accepting connection from client
  Server(boost::asio::io_service& io_service) : io_service_(io_service),
  acceptor_(io_service_, tcp::endpoint(tcp::v4(), 8888))
  {      
    info[0] = '2';
    info[1] = '3';
    start_accept();
  }
};
/*
int main(int argc, char *argv[])
{
  try 
{

    boost::asio::io_service io_service;   
    Server server(io_service);
    io_service.run();

} 
catch(std::exception& e) 
{
    std::cerr << e.what() << endl;
}
  while(1){
  }
  return 0;

}
 */
