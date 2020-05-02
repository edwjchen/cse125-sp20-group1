#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
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
#include <boost/foreach.hpp>

#include "chat_message.hpp"

using namespace boost::asio;
using ip::tcp;
using std::cout;
using std::endl;

class Server : public boost::enable_shared_from_this<Server>
{
private:
    boost::asio::io_service& io_service_;
    tcp::acceptor acceptor_;
    int i = 0;

    char info_buffer[1024];

    std::vector<float> v;

    chat_message obj;

    void send_info(int id, std::shared_ptr<tcp::socket> socket){
        while(1){
            std::string msg = obj.data()+'\n';
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
            std::string key_op = "";
            std::string mouse_op;

            // Read JSON from client
            try{
                if(data != ""){
                    stringstream ss;
                    ss << data;

                    pt::ptree tar;
                    pt::read_json(ss, tar);

                    int i = 0;
                    BOOST_FOREACH(const pt::ptree::value_type& child, tar.get_child("cmd")) {
                        if(i == 0){
                            key_op = child.second.get<std::string>("key");
                            cout << key_op << endl;
                        }
                        else{
                            // Mouse cmd

                        }
                        i++;
                    }

                    // print heightmap passed from client
                    // cout << "printing Height_map" << endl;
                    // BOOST_FOREACH(const pt::ptree::value_type& v, tar.get_child("Height_map")) {
                    //     cout << v.second.data();
                    // }
                    // cout << "-------------------" << endl;

                }
            } catch (...){

            }

            if(key_op != ""){
                cout << "id: " << id << ", operation: "<< key_op << endl;
                if(id == 1){
                    obj.update1(key_op.at(0));
                }else if(id == 2){
                    obj.update2(key_op.at(0));
                }
            }
        }
    }

    void start_accept()
    {
        // socket
        cout << "begin accepting" << endl;
        while (i < 4){
            boost::asio::io_service io_service;
            tcp::socket socket_(io_service);
            std::shared_ptr<tcp::socket> socket_1(new tcp::socket(io_service));
            acceptor_.accept(*socket_1);
            i++;

            cout << "accepted: " << i << endl;
            boost::asio::write( *socket_1, boost::asio::buffer(std::to_string(i)+'\n') );
            //update(i, socket_1);
            boost::thread send_thread(&Server::send_info, this, i, socket_1);
            boost::thread read_thread(&Server::read_info, this, i, socket_1);
        }
        while(1){}
    }

public:
    //constructor for accepting connection from client
    Server(boost::asio::io_service& io_service) : io_service_(io_service),
            acceptor_(io_service_, tcp::endpoint(tcp::v4(), 8888))
    {
        for (int i = 0; i < 16300; i++){
            v.push_back(i);
        }
        start_accept();
    }
};

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
