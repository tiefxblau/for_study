#include "Socket.hpp"
#include "Reactor.hpp"
#include "Accepter.hpp"
#include "Util.hpp"

static void Usage(char* proc)
{
    std::cerr << "Usage:\n\t" << proc << " server_port" << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        return 1;
    }
    int port = atoi(argv[1]);

    int listen_sock = Sock::Socket();
    Sock::Bind(listen_sock, port);
    Sock::Listen(listen_sock);
    SetNoBlock(listen_sock);

    Reactor R;

    Event ev;
    ev.fd = listen_sock;
    ev.R = &R;

    ev.RegisterCallBack(Accepter(), CallBack(), CallBack());

    R.InsertEvent(std::move(ev), EPOLLIN | EPOLLET);

    R.Dispatcher();
}