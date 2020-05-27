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
#include <boost/archive/text_oarchive.hpp>

#include "Terrain.hpp"
#include "GameManager.hpp"

using namespace boost::asio;
using ip::tcp;
using std::cout;
using std::endl;

namespace pt = boost::property_tree;

class Server : public boost::enable_shared_from_this<Server>
{
private:
    boost::asio::io_service& io_service_;
    tcp::acceptor acceptor_;
    vector <std::shared_ptr<tcp::socket>> sockets;
    int i = 0;

    GameManager gm;

    void send_info(int id, std::shared_ptr<tcp::socket> socket){
        while(1){

            if(sockets[id-1] == nullptr){
                return;
            }
            if(gm.UpdateTime()){
                boost::asio::streambuf buf;
                std::ostream os( &buf );
                boost::archive::text_oarchive ar( os );
                ar & gm;
                os << '\n';
                boost::asio::write( *socket, boost::asio::buffer(buf.data()));
                //std::string msg = gm.encode();
                //boost::asio::write( *socket, boost::asio::buffer(msg) );
                if(gm.updateTerrain){
                    cout << "111" << endl;
                    gm.updateTerrain = false;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(30));
            }
            else{
                gm.updateStatus("end");
                boost::asio::streambuf buf;
                std::ostream os( &buf );
                boost::archive::text_oarchive ar( os );
                ar & gm;
                os << '\n';
                boost::asio::write( *socket, boost::asio::buffer(buf.data()));
                std::this_thread::sleep_for(std::chrono::milliseconds(60000));
            }
        }
    }

    void read_info(int id, std::shared_ptr<tcp::socket> socket)
    {
        while(1){
            boost::asio::streambuf buf;
            boost::system::error_code ec;
            boost::asio::read_until( *socket, buf, "\n" , ec);
            
            // TODO: handle player exits
            if(ec ==  boost::asio::error::eof){
                cout << "player "<< id << " exit" << endl;
                sockets[id-1]->close();
                sockets[id-1] = nullptr;
                break;
            }

            std::string data = boost::asio::buffer_cast<const char*>(buf.data());
            gm.handle_input(data, id);
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
            sockets.push_back(socket_1);
            //update(i, socket_1);
            // boost::thread send_thread(&Server::send_info, this, i, socket_1);
            // boost::thread read_thread(&Server::read_info, this, i, socket_1);
            addPlayer();
        }
        gm.updateStatus("update");
        cout << "4 players ready" << endl;
         for(int j=0;j<4;j++){
             boost::thread send_thread(&Server::send_info, this, j+1, sockets[j]);
             boost::thread read_thread(&Server::read_info, this, j+1, sockets[j]);
         }
        while(1){}
    }
    void addPlayer(){
        gm.player++;
        gm.updateStatus("wait");
        for(int j = 0; j < gm.player; j++){
            boost::asio::streambuf buf;
            std::ostream os( &buf );
            boost::archive::text_oarchive ar( os );
            ar & gm;
            os << '\n';
            boost::asio::write( *sockets[j], boost::asio::buffer(buf.data()));
        }
    }

public:
    //constructor for accepting connection from client
    Server(boost::asio::io_service& io_service) : io_service_(io_service),
            acceptor_(io_service_, tcp::endpoint(tcp::v4(), 8888))
    {
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