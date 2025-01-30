#include <iostream> /* cerr */
#include <algorithm>
#include "supservidor.h"

using namespace std;

/* ========================================
   CLASSE SUPSERVIDOR
   ======================================== */

/// Construtor
SupServidor::SupServidor()
    : Tanks(), server_on(false), LU()
      /*ACRESCENTADO*/
      ,
      socket_server()
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
    mysocket_queue fila;
    mysocket_status resultado;
    tcp_mysocket socket_temp;
    iteradorUser iterUser;
    SupState estadoTanque;
    uint16_t comando, Open, Input;
    string login, password;

    while (server_on)
    {
        try
        {
            if (!socket_server.accepting())
            {
                throw "socket de conexoes fechado";
            }

            fila.clear();
            fila.include(socket_server);
            for (auto &U : LU)
            {
                if (U.isConnected())
                    fila.include(U.socket);
            }

            resultado = fila.wait_read(SUP_TIMEOUT * 100);

            switch (resultado)
            {
            case mysocket_status::SOCK_TIMEOUT:
                break;
            case mysocket_status::SOCK_ERROR:
                throw "ERRO NO SELECT";
            case mysocket_status::SOCK_OK:
                try
                {
                    for (iterUser = LU.begin(); iterUser != LU.end(); ++iterUser)
                    {
                        if (server_on && iterUser->isConnected() && fila.had_activity(iterUser->socket))
                        {
                            resultado = iterUser->socket.read_uint16(comando);
                            if (resultado != mysocket_status::SOCK_OK)
                                throw "ERRO";

                            switch (comando)
                            {
                            case CMD_GET_DATA:
                                iterUser->socket.write_uint16(CMD_DATA);
                                readStateFromSensors(estadoTanque);
                                iterUser->socket.write_bytes((uint8_t *)&estadoTanque, sizeof(estadoTanque));
                                break;

                            case CMD_SET_V1:
                                iterUser->socket.write_uint16(CMD_SET_V1);
                                resultado = iterUser->socket.read_uint16(Open, 1000 * SUP_TIMEOUT);
                                if (resultado == mysocket_status::SOCK_OK && iterUser->isAdmin)
                                {
                                    setV1Open(Open != 0);
                                    iterUser->socket.write_uint16(CMD_OK);
                                }
                                else
                                {
                                    iterUser->socket.write_uint16(CMD_ERROR);
                                    throw "PERMISSAO NEGADA";
                                }
                                break;

                            case CMD_SET_V2:
                                iterUser->socket.write_uint16(CMD_SET_V2);
                                resultado = iterUser->socket.read_uint16(Open, 1000 * SUP_TIMEOUT);
                                if (resultado == mysocket_status::SOCK_OK && iterUser->isAdmin)
                                {
                                    setV2Open(Open != 0);
                                    iterUser->socket.write_uint16(CMD_OK);
                                }
                                else
                                {
                                    iterUser->socket.write_uint16(CMD_ERROR);
                                    throw "PERMISSAO NEGADA";
                                }
                                break;

                            case CMD_SET_PUMP:
                                resultado = iterUser->socket.read_uint16(Input, 1000 * SUP_TIMEOUT);
                                if (resultado != mysocket_status::SOCK_OK)
                                    throw "ERRO";
                                setPumpInput(Input);
                                iterUser->socket.write_uint16(CMD_OK);
                                break;

                            case CMD_LOGOUT:
                                cout << "USUARIO: " << iterUser->login << " SE DESCONECTOU " << endl;
                                iterUser->close();
                                break;

                            default:
                                throw "COMANDO INVALIDO";
                            }
                        }
                    }
                }
                catch (const char *err)
                {
                    cerr << "Erro do usuario: " << err << endl;
                    iterUser->close();
                }

                if (server_on && fila.had_activity(socket_server))
                {
                    resultado = socket_server.accept(socket_temp);
                    if (resultado != mysocket_status::SOCK_OK)
                        throw "ERRO AO ACEITAR CONEXAO";

                    try
                    {
                        resultado = socket_temp.read_uint16(comando, SUP_TIMEOUT * 1000);
                        if (resultado != mysocket_status::SOCK_OK || comando != CMD_LOGIN)
                            throw "ERRO DE LOGIN";

                        resultado = socket_temp.read_string(login, SUP_TIMEOUT * 1000);
                        if (resultado != mysocket_status::SOCK_OK)
                            throw "ERRO LOGIN";
                        resultado = socket_temp.read_string(password, SUP_TIMEOUT * 1000);
                        if (resultado != mysocket_status::SOCK_OK)
                            throw "ERRO SENHA";

                        if (login.size() < 6 || login.size() > 12 || password.size() < 6 || password.size() > 12)
                            throw "LOGIN INVALIDO";

                        iterUser = find(LU.begin(), LU.end(), login);
                        if (iterUser == LU.end())
                            throw "USUARIO NAO EXISTE";
                        if (iterUser->password != password)
                            throw "SENHA INCORRETA";
                        if (iterUser->isConnected())
                            throw "USUARIO JA CONECTADO";

                        iterUser->socket.swap(socket_temp);
                        iterUser->socket.write_uint16(iterUser->isAdmin ? CMD_ADMIN_OK : CMD_OK);

                        cout << "USUARIO: " << iterUser->login << " SE CONECTOU COM SUCESSO " << endl;
                    }
                    catch (const char *err)
                    {
                        cerr << "Erro na conexao com o usuario: " << err << endl;
                        socket_temp.close();
                    }
                }
                break;
            }
        }
        catch (const char *err)
        {
            cerr << "Erro no servidor: " << err << endl;
            server_on = false;
            for (auto &U : LU)
                U.close();
            socket_server.close();
        }
    }
    cout << "SERVIDOR ENCERRADO" << endl;
}
