#include <iostream> /* cerr */
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
    if (/*MODIFICADO*/ mysocket::init() != mysocket_status::SOCK_OK)
    {
        cerr << "Biblioteca mysocket nao pode ser inicializada";
        exit(-1);
    }
}

/// Destrutor
SupServidor::~SupServidor()
{
    // Deve parar a thread do servidor
    server_on = false;

    // Fecha todos os sockets dos clientes
    for (auto &U : LU)
        U.close();
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
    if (server_on)
        return true;

    // Liga os tanques
    setTanksOn();

    // Indica que o servidor estah ligado a partir de agora
    server_on = true;

    try
    {
        // Coloca o socket de conexoes em escuta
        /*ACRESCENTEI*/
        // Em caso de erro, gera excecao
        if (/*MODIFICADO*/ socket_server.listen("23456") != mysocket_status::SOCK_OK)
            throw 1;

        // Lanca a thread do servidor que comunica com os clientes
        /*ACRESCENTEI*/
        thr_server = std::thread(&SupServidor::thr_server_main, this);
        // Em caso de erro, gera excecao
        if (/*MODIFICADO*/ !thr_server.joinable())
            throw 2;
    }
    catch (int i)
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
    if (!server_on)
        return;

    // Deve parar a thread do servidor
    server_on = false;

    // Fecha todos os sockets dos clientes
    for (auto &U : LU)
        U.close();
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
void SupServidor::readStateFromSensors(SupState &S) const
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
    for (const auto &U : LU)
    {
        cout << U.login << '\t'
             << "Admin=" << (U.isAdmin ? "SIM" : "NAO") << '\t'
             << "Conect=" << (U.isConnected() ? "SIM" : "NAO") << '\n';
    }
}

/// Adicionar um novo usuario
bool SupServidor::addUser(const string &Login, const string &Senha,
                          bool Admin)
{
    // Testa os dados do novo usuario
    if (Login.size() < 6 || Login.size() > 12)
        return false;
    if (Senha.size() < 6 || Senha.size() > 12)
        return false;

    // Testa se jah existe usuario com mesmo login
    auto itr = find(LU.begin(), LU.end(), Login);
    if (itr != LU.end())
        return false;

    // Insere
    LU.push_back(User(Login, Senha, Admin));

    // Insercao OK
    return true;
}

/// Remover um usuario
bool SupServidor::removeUser(const string &Login)
{
    // Testa se existe usuario com esse login
    auto itr = find(LU.begin(), LU.end(), Login);
    if (itr == LU.end())
        return false;

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
    mysocket_queue queue;

    while (server_on)
    {
        try
        {
            // Verifica se o socket principal do servidor foi fechado, lançando uma exceção se for o caso
            if (socket_server.closed())
            {
                throw "socket de conexoes fechados";
            }

            // Limpa a fila de sockets antes de incluir novos sockets para monitoramento
            queue.clear();
            // Inclui o socket principal do servidor na fila para monitorar novas conexões
            queue.include(socket_server);
            // Itera sobre todos os usuários conectados e inclui os sockets deles na fila se estiverem conectados
            for (auto &U : LU)
            {
                if (U.isConnected())
                {
                    queue.include(U.socket);
                }
            }

            // Espera até 1 segundo para detectar atividade em algum dos sockets na fila
            mysocket_status status = queue.wait_read(1000); // Timeout de 1 segundo

            // Verifica se o tempo limite foi atingido sem atividade em nenhum socket
            if (status == mysocket_status::SOCK_TIMEOUT)
            {
                continue;
            }
            // Verifica se ocorreu um erro na fila de sockets e encerra o servidor em caso positivo
            else if (status == mysocket_status::SOCK_ERROR)
            {
                throw "erro na fila de sockets";
            }
            // Processa a atividade detectada em algum socket da fila
            else if (status == mysocket_status::SOCK_OK)
            {
                // Testa se houve atividade nos sockets dos clientes
                for (auto &U : LU)
                {
                    if (U.isConnected() && queue.had_activity(U.socket))
                    {
                        // Tenta ler um comando do socket do cliente e desconecta o cliente em caso de erro
                        uint16_t cmd;
                        if (U.socket.read_uint16(cmd) != mysocket_status::SOCK_OK)
                        {
                            std::cout << U.login << ": Desconectado (Erro na leitura do comando)" << std::endl;
                            U.close();
                            continue;
                        }

                        // Processa o comando recebido
                        switch (cmd)
                        {
                        case CMD_GET_DATA:
                        {
                            // Envia o estado dos tanques para o cliente
                            U.socket.write_uint16(CMD_DATA);
                            SupState estadoTanque;
                            readStateFromSensors(estadoTanque);
                            U.socket.write_bytes((uint8_t *)&estadoTanque, sizeof(estadoTanque));
                            U.socket.write_uint16(CMD_OK);
                            break;
                        }
                        case CMD_SET_V1:
                        {
                            // Ajusta o estado da válvula V1 enviado pelo cliente
                            uint16_t state;
                            if (U.socket.read_uint16(state) != mysocket_status::SOCK_OK || !U.isAdmin)
                            {
                                U.socket.write_uint16(CMD_ERROR);
                                break;
                            }
                            setV1Open(state != 0);
                            U.socket.write_uint16(CMD_OK);
                            break;
                        }
                        case CMD_LOGOUT:
                        {
                            // Processa o comando de logout do cliente
                            std::cout << U.login << ": Logout realizado" << std::endl;
                            U.close();
                            break;
                        }
                        default:
                        {
                            // Comando inválido enviado pelo cliente
                            U.socket.write_uint16(CMD_ERROR);
                            std::cout << U.login << ": Enviou comando inválido" << std::endl;
                            break;
                        }
                        }
                    }
                }

                // Verifica se houve atividade no socket principal do servidor para aceitar novas conexões
                if (queue.had_activity(socket_server))
                {
                    tcp_mysocket temp_socket;
                    if (socket_server.accept(temp_socket) == mysocket_status::SOCK_OK)
                    {
                        // Processa uma nova conexão de cliente
                        uint16_t loginCmd;
                        std::string login, password;

                        // Lê os dados de login e senha do cliente
                        if (temp_socket.read_uint16(loginCmd) != mysocket_status::SOCK_OK ||
                                temp_socket.read_string(login) != mysocket_status::SOCK_OK ||
                                temp_socket.read_string(password) != mysocket_status::SOCK_OK)
                        {
                            temp_socket.close();
                            std::cout << "Erro ao receber dados de login e senha do cliente" << std::endl;
                            continue;
                        }

                        // Verifica se o comando de login é válido
                        if (loginCmd != CMD_LOGIN)
                        {
                            temp_socket.write_uint16(CMD_ERROR);
                            temp_socket.close();
                            std::cout << "Comando inválido recebido do cliente: " << loginCmd << std::endl;
                            continue;
                        }

                        // Procura pelo login do cliente na lista de usuários para autenticação
                        auto itr = std::find(LU.begin(), LU.end(), login);
                        if (itr != LU.end() && itr->password == password)
                        {
                            itr->socket = std::move(temp_socket);
                            std::cout << login << ": Conectado com sucesso" << std::endl;

                            // Envia uma resposta de sucesso ao cliente
                            uint16_t response = itr->isAdmin ? CMD_ADMIN_OK : CMD_OK;
                            itr->socket.write_uint16(response);
                        }
                        else
                        {
                            // Envia uma resposta de erro ao cliente em caso de falha no login
                            temp_socket.write_uint16(CMD_ERROR);
                            temp_socket.close();
                            std::cout << login << ": Falha no login. Usuário ou senha incorretos" << std::endl;
                        }
                    }
                }
            }
        }
        catch (const char *err)
        {
            // Trata erros graves que encerram o servidor
            cerr << "Erro no servidor: " << err << endl;

            // Define o servidor como desligado
            server_on = false;

            // Fecha as conexões com todos os clientes
            for (auto &U : LU)
            {
                cout << "Fechando conexão do usuário: " << U.login << endl;
                U.close();
            }

            // Fecha o socket principal do servidor
            socket_server.close();
        }
    }
}

