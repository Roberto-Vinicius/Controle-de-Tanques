#include <iostream>     /* cerr */
#include <algorithm>
#include "supservidor.h"

using namespace std;

/* ========================================
   CLASSE SUPSERVIDOR
   ======================================== */

/// Construtor
SupServidor::SupServidor()
    : Tanks()
    , server_on(false)
    , LU()
      /*ACRESCENTADO*/
    , socket_server()
{
    // Inicializa a biblioteca de sockets
    /*ACRESCENTEI*/
    // Em caso de erro, mensagem e encerra
    if (/*MODIFICADO*/mysocket::init() != mysocket_status::SOCK_OK)
    {
        cerr <<  "Biblioteca mysocket nao pode ser inicializada";
        exit(-1);
    }
}

/// Destrutor
SupServidor::~SupServidor()
{
    // Deve parar a thread do servidor
    server_on = false;

    // Fecha todos os sockets dos clientes
    for (auto& U : LU) U.close();
    // Fecha o socket de conexoes
    /*ACRESCENTEI*/
    socket_server.close();

    // Espera o fim da thread do servidor
    /*ACRESCENTEI*/
    if (thr_server.joinable())
    {
        thr_server.join();
    }

    // Encerra a biblioteca de sockets
    /*ACRESCENTEi*/
    mysocket::end();
}

/// Liga o servidor
bool SupServidor::setServerOn()
{
    // Se jah estah ligado, nao faz nada
    if (server_on) return true;

    // Liga os tanques
    setTanksOn();

    // Indica que o servidor estah ligado a partir de agora
    server_on = true;

    try
    {
        // Coloca o socket de conexoes em escuta
        /*ACRESCENTEI*/
        // Em caso de erro, gera excecao
        if (/*MODIFICADO*/socket_server.listen("23456") != mysocket_status::SOCK_OK) throw 1;

        // Lanca a thread do servidor que comunica com os clientes
        /*ACRESCENTEI*/
        thr_server = std::thread(&SupServidor::thr_server_main, this);
        // Em caso de erro, gera excecao
        if (/*MODIFICAR*/false) throw 2;
    }
    catch(int i)
    {
        cerr << "Erro " << i << " ao iniciar o servidor\n";

        // Deve parar a thread do servidor
        server_on = false;

        // Fecha o socket do servidor
        /*ACRESCENTEI*/
        socket_server.close();

        return false;
    }

    // Tudo OK
    return true;
}

/// Desliga o servidor
void SupServidor::setServerOff()
{
    // Se jah estah desligado, nao faz nada
    if (!server_on) return;

    // Deve parar a thread do servidor
    server_on = false;

    // Fecha todos os sockets dos clientes
    for (auto& U : LU) U.close();
    // Fecha o socket de conexoes
    /*ACRESCENTEI*/
    socket_server.close();

    // Espera pelo fim da thread do servidor
    /*ACRESCENTEI*/
    if (thr_server.joinable())
    {
        thr_server.join();
    }

    // Faz o identificador da thread apontar para thread vazia
    /*ACRESCENTEI*/
    thr_server = std::thread();

    // Desliga os tanques
    setTanksOff();
}

/// Leitura do estado dos tanques
void SupServidor::readStateFromSensors(SupState& S) const
{
    // Estados das valvulas: OPEN, CLOSED
    S.V1 = v1isOpen();
    S.V2 = v2isOpen();
    // Niveis dos tanques: 0 a 65535
    S.H1 = hTank1();
    S.H2 = hTank2();
    // Entrada da bomba: 0 a 65535
    S.PumpInput = pumpInput();
    // Vazao da bomba: 0 a 65535
    S.PumpFlow = pumpFlow();
    // Estah transbordando (true) ou nao (false)
    S.ovfl = isOverflowing();
}

/// Leitura e impressao em console do estado da planta
void SupServidor::readPrintState() const
{
    if (tanksOn())
    {
        SupState S;
        readStateFromSensors(S);
        S.print();
    }
    else
    {
        cout << "Tanques estao desligados!\n";
    }
}

/// Impressao em console dos usuarios do servidor
void SupServidor::printUsers() const
{
    for (const auto& U : LU)
    {
        cout << U.login << '\t'
             << "Admin=" << (U.isAdmin ? "SIM" : "NAO") << '\t'
             << "Conect=" << (U.isConnected() ? "SIM" : "NAO") << '\n';
    }
}

/// Adicionar um novo usuario
bool SupServidor::addUser(const string& Login, const string& Senha,
                          bool Admin)
{
    // Testa os dados do novo usuario
    if (Login.size()<6 || Login.size()>12) return false;
    if (Senha.size()<6 || Senha.size()>12) return false;

    // Testa se jah existe usuario com mesmo login
    auto itr = find(LU.begin(), LU.end(), Login);
    if (itr != LU.end()) return false;

    // Insere
    LU.push_back( User(Login,Senha,Admin) );

    // Insercao OK
    return true;
}


/// Remover um usuario
bool SupServidor::removeUser(const string& Login)
{
    // Testa se existe usuario com esse login
    auto itr = find(LU.begin(), LU.end(), Login);
    if (itr == LU.end()) return false;

    // Remove
    LU.erase(itr);

    // Remocao OK
    return true;
}

/// A thread que implementa o servidor.
/// Comunicacao com os clientes atraves dos sockets.
void SupServidor::thr_server_main(void)
{
    // Fila de sockets para aguardar chegada de dados
    /*ACRESCENTEI*/
    mysocket_queue queue;

    while (server_on)
    {
        // Erros mais graves que encerram o servidor
        // Parametro do throw e do catch eh uma const char* = "texto"
        try
        {
            // Encerra se o socket de conexoes estiver fechado
            if (/*MODIFICADO*/socket_server.closed())
            {
                throw "socket de conexoes fechado";
            }

            // Inclui na fila de sockets todos os sockets que eu
            // quero monitorar para ver se houve chegada de dados

            // Limpa a fila de sockets
            /*ACRESCENTEI*/
            queue.clear();
            // Inclui na fila o socket de conexoes
            /*ACRESCENTEI*/
            queue.include(socket_server);
            // Inclui na fila todos os sockets dos clientes conectados
            /*ACRESCENTADO*/
            for (auto& U : LU)
            {
                if (U.isConnected())
                {
                    queue.include(U.socket);
                }
            }

            // Espera ateh que chegue dado em algum socket (com timeout)
            /*ACRESCENTADO*/
            mysocket_status status = queue.wait_read(1000); // Timeout de 1 segundo

            // EDITADO
            // De acordo com o resultado da espera:
            if (status == mysocket_status::SOCK_TIMEOUT)
            {
                // Saiu por timeout: não houve atividade em nenhum socket
                continue;
            }
            else if (status == mysocket_status::SOCK_ERROR)
            {
                // Erro no select: encerra o servidor
                throw "erro na fila de sockets";
            }
            else if (status == mysocket_status::SOCK_OK)
            {
                // Houve atividade em algum socket da fila:
                // Testa se houve atividade nos sockets dos clientes
                for (auto& U : LU)
                {
                    if (U.isConnected() && queue.had_activity(U.socket))
                    {
                        // Lê o comando do cliente
                        uint16_t cmd;
                        if (U.socket.read_uint16(cmd) != mysocket_status::SOCK_OK)
                        {
                            // Exibe mensagem de desconexão
                            std::cout << U.login << ": Desconectado" << std::endl; // Nova linha adicionada
                            U.close(); // Fecha o socket do cliente com erro
                            continue;
                        }

                        // Processa o comando recebido (implemente a lógica aqui)
                        // Exemplo: enviar resposta
                        uint16_t response = CMD_OK; // Simulação de resposta
                        U.socket.write_uint16(response);
                    }
                }

                // Testa se houve atividade no socket de conexão
                if (queue.had_activity(socket_server))
                {
                    // Estabelece nova conexão em socket temporário
                    tcp_mysocket temp_socket;
                    if (socket_server.accept(temp_socket) == mysocket_status::SOCK_OK)
                    {
                        // Lê comando, login e senha
                        uint16_t loginCmd;
                        std::string login, password;

                        if (temp_socket.read_uint16(loginCmd) != mysocket_status::SOCK_OK ||
                                temp_socket.read_string(login) != mysocket_status::SOCK_OK ||
                                temp_socket.read_string(password) != mysocket_status::SOCK_OK)
                        {
                            temp_socket.close(); // Fecha o socket com erro
                            continue;
                        }

                        // Testa usuário
                        auto itr = std::find(LU.begin(), LU.end(), login);
                        if (itr != LU.end() && itr->password == password)
                        {
                            // Se deu tudo certo, faz o socket temporário ser o novo socket do cliente
                            itr->socket = std::move(temp_socket);

                            // Exibe mensagem de conexão
                            std::cout << login << ": Conectado" << std::endl; // Nova linha adicionada

                            uint16_t response = CMD_OK; // Resposta de sucesso
                            itr->socket.write_uint16(response);
                        }
                        else
                        {
                            // Login ou senha incorretos
                            uint16_t response = CMD_ERROR;
                            temp_socket.write_uint16(response);
                            temp_socket.close();
                        }
                    }
                }
            }

        } // fim try - Erros mais graves que encerram o servidor
        catch(const char* err)  // Erros mais graves que encerram o servidor
        {
            cerr << "Erro no servidor: " << err << endl;

            // Sai do while e encerra a thread
            server_on = false;

            // Fecha todos os sockets dos clientes
            for (auto& U : LU) U.close();
            // Fecha o socket de conexoes
            /*ACRESCENTADO*/
            socket_server.close();

            // Os tanques continuam funcionando

        } // fim catch - Erros mais graves que encerram o servidor
    } // fim while (server_on)
}
