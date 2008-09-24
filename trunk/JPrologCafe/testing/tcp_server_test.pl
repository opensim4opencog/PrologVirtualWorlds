:- use_module(library(streampool)).
:- ['logicmoo.swi'].

streampool_server(Port) :-
        tcp_socket(ServerSocket),
        catch(ignore(tcp_setopt(ServerSocket, reuseaddr)),_,true),
        tcp_bind(ServerSocket, Port),
        tcp_listen(ServerSocket, 5),
        tcp_open_socket(ServerSocket, In, _Out),
        add_stream_to_pool(In, streampool_accept(ServerSocket)),
        stream_pool_main_loop.

streampool_accept(ServerSocket) :-
        tcp_accept(ServerSocket, AcceptFd, GETIP),
        tcp_open_socket(AcceptFd, In, Out),
        add_stream_to_pool(In, streampool_servant(In, Out, GETIP)).

streampool_servant(In, Out, _Peer) :-
        read_line_to_codes(In, Command),
        close(In),
        format(Out, 'Please to meet you: ~s~n', [Command]),
        close(Out),
        delete_stream_from_pool(In).

:-streampool_server(666).
