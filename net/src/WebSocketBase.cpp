#include <WebSocketBase.hpp>
#include <websocketpp/frame.hpp>

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

mutex mu;

void WebSocketBase::init() {
    // Set logging to be pretty verbose (everything except message payloads)
    c.set_access_channels(websocketpp::log::alevel::all);
    c.clear_access_channels(websocketpp::log::alevel::frame_payload);

    // Initialize ASIO
    c.init_asio();

    // Register our message handler
    c.set_message_handler(websocketpp::lib::bind(&WebSocketBase::on_message, this, ::_1, ::_2));
    c.set_open_handler(websocketpp::lib::bind(&WebSocketBase::on_open, this, ::_1));
    c.set_close_handler(websocketpp::lib::bind(&WebSocketBase::on_close, this, ::_1));
    c.set_fail_handler(websocketpp::lib::bind(&WebSocketBase::on_fail, this, ::_1));
    c.start_perpetual();
    thread_ = websocketpp::lib::make_shared<websocketpp::lib::thread>(&client::run, &c);
}

void WebSocketBase::connect() {
    if (status != not_connect) {
        return;
    }
    mu.lock();
    status = connecting;
    try {
        std::cout << "connect to url=" << url << std::endl;

        websocketpp::lib::error_code ec;
        client::connection_ptr con = c.get_connection(url, ec);
        if (ec) {
            std::cout << "could not create connection because: " << ec.message() << std::endl;
            return ;
        }

        hdl = con->get_handle();
        // Note that connect here only requests a connection. No network messages are
        // exchanged until the event loop starts running in the next line.
        c.connect(con);

        // Start the ASIO io_service run loop
        // this will cause a single connection to be made to the server. c.run()
        // will exit when this connection is closed.
        //c.run();
    }
    catch (websocketpp::exception const& e) {
        std::cout << e.what() << std::endl;
    }
    mu.unlock();
}

void WebSocketBase::close() {
    c.close(hdl, websocketpp::close::status::normal, "close manual");
}

void WebSocketBase::terminate(){
    c.stop_perpetual();
    thread_->join();
}


void WebSocketBase::on_open(websocketpp::connection_hdl hdl) {
    this->status = connected;
}


void WebSocketBase::on_fail(websocketpp::connection_hdl hdl) {
    if (this->status == connecting) {
        std::cout << "coonect fail\n";
        this->status = not_connect;
    }
}

void WebSocketBase::on_close(websocketpp::connection_hdl hdl) {
    this->status = not_connect;
    after_close();
}

void WebSocketBase::on_message(websocketpp::connection_hdl hdl, message_ptr msg) {
    if (msg->get_opcode() == websocketpp::frame::opcode::binary) {
        std::cout << "Receive byte data." << std::endl;
    }
    else {
        process_message(msg->get_payload());
    }
}

void WebSocketBase::send_message(std::string const& payload) {
    if (this->status == connected) {
        c.send(hdl, payload, websocketpp::frame::opcode::text);
    }
}

void WebSocketBase::send_message(void const* payload, size_t len, websocketpp::lib::error_code& ec) {
    if(this->status == connected) {
        c.send(hdl, (void*)payload, len, websocketpp::frame::opcode::binary, ec);
    }
}

void WebSocketBase::process_message(std::string const& payload) {
    std::cout << "Recieve text message :" << payload <<std::endl;
}

void WebSocketBase::after_close() {
    std::cout << "WebSocketBase::after_close\n";
}