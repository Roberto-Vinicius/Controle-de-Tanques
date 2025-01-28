#include <iostream>
#include "supcliente.h"
#include <thread>
#include <chrono>

/// Construtor default
SupCliente::SupCliente()
    : meuUsuario(""), encerrarCliente(true), is_admin(false), last_S(), start_t(time_t(-1)), last_t(time_t(-1)), timeRefresh(20)
      /* ACRESCENTAR */
      ,
      sock(), mtx(), thr()
{
    // Inicializa a biblioteca de sockets
    /* ACRESCENTAR */
    if (/* MODIFICADO */ mysocket::init() != mysocket_status::SOCK_OK)
    {
        std::cerr << "Biblioteca mysocket nao pode ser inicializada";
        exit(-666);
    }
}

/// Destrutor
SupCliente::~SupCliente()
{
    // Nao pode chamar a funcao "desconectar" pois ela chama uma funcao virtual pura,
    // o que nao deve ocorrer no destrutor

    // Testa se estah conectado
    if (isConnected())
    {
        // Envia o comando de logout para o servidor
        /* ACRESCENTEI */
        uint16_t logoutCmd = CMD_LOGOUT;
        if (sock.write_uint16(logoutCmd) != mysocket_status::SOCK_OK)
        {
            std::cerr << "Erro ao enviar comando de logout." << std::endl;
        }
        // Espera 1 segundo para dar tempo ao servidor de ler a msg de LOGOUT
        // antes de fechar o socket
        /* ACRESCENTEI */
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        // Fecha o socket e, consequentemente, deve
        // encerrar a thread de leitura de dados do socket
        /* ACRESCENTEI */
        sock.close();
    }

    // Aguarda pelo fim da thread de recepcao
    join_if_joinable();

    // Encerra a biblioteca de sockets
    /* ACRESCENTEI */
    mysocket::end();
}

/// Conecta com o servidor.
/// Esta funcao soh pode ser chamada do programa principal,
/// jah que ela lanca a thread do cliente
void SupCliente::conectar(const std::string &IP,
                          const std::string &Login,
                          const std::string &Senha)
{
    // Comando recebido
    uint16_t cmd;

    try
    {
        // Soh conecta se nao estiver conectado
        if (isConnected())
            throw 101;

        // Conecta o socket
        // Em caso de erro, throw 102
        /* ACRESCENTEI */
        if (sock.connect(IP, "23456") != mysocket_status::SOCK_OK)
        {
            throw 102; // Erro ao conectar
        }

        // Envia o comando CMD_LOGIN.
        // Nao precisa bloquear o mutex para garantir exclusao mutua
        // pq nesse momento ainda nao foi lancada a thread.
        // Entao, essa funcao eh a unica enviando dados pelo socket.
        // Em caso de erro, throw 103
        std::cout << "Enviando comando de login ao servidor..." << std::endl;

        uint16_t loginCmd = CMD_LOGIN;
        if (sock.write_uint16(loginCmd) != mysocket_status::SOCK_OK)
        {
            std::cerr << "Erro: Falha ao enviar o comando de login para o servidor." << std::endl;
            throw 103; // Código de erro mantido para consistência
        }

        // Envia 1o parametro do comando (login)
        // Em caso de erro, throw 104
        /* ACRESCENTEI */
        if (sock.write_string(Login) != mysocket_status::SOCK_OK)
        {
            throw 104; // Erro ao enviar login
        }
        // Envia 2o parametro do comando (senha)
        // Em caso de erro, throw 105
        /* ACRESCENTEI */
        if (sock.write_string(Senha) != mysocket_status::SOCK_OK)
        {
            throw 105; // Erro ao enviar senha
        }

        // Leh a resposta (cmd) do servidor ao pedido de conexao
        // Em caso de erro, throw 106
        /* ACRESCENTEi */
        if (sock.read_uint16(cmd) != mysocket_status::SOCK_OK)
        {
            throw 106; // Erro ao receber resposta do servidor
        }
        // Se a resposta nao for CMD_ADMIN_OK ou CMD_OK, throw 107
        if (cmd != CMD_ADMIN_OK && cmd != CMD_OK)
        {
            std::cerr << "Erro: Resposta inesperada do servidor. Código recebido: " << cmd << std::endl;
            throw 107; // Resposta inválida do servidor
        }
        // Armazena o nome do usuario
        meuUsuario = Login;
        // Eh administrador (de acordo com resposta do servidor)?
        is_admin = (cmd == CMD_ADMIN_OK);
        // Cliente em funcionamento
        encerrarCliente = false;

        std::cout << "Conexão estabelecida com sucesso. Usuário: " << Login
                  << (is_admin ? " (Administrador)" : " (Usuário padrão)") << std::endl;

        // Lanca a thread de solicitacao periodica de dados
        // Em caso de erro, throw 108
        /* ACRESCENTAR */
        try
        {
            thr = std::thread(&SupCliente::main_thread, this);
        }
        catch (const std::system_error &e)
        {
            throw 108;
        }
    }
    catch (int err)
    {
        // Encerra o cliente
        encerrarCliente = true;
        // Fecha o socket
        /* ACRESCENTEI */
        try
        {
            sock.close();
            std::cerr << "Socket fechado com após erro." << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Erro ao fechar o socket: " << e.what() << std::endl;
        }

        // Mensagem de erro detalhada para debug
        std::string msg_err = "Erro na conexão com o servidor. IP: " + IP +
                              ", usuário: " + Login +
                              ", código do erro: " + std::to_string(err);

        // Exibe msg de erro
        virtExibirErro(msg_err);
        std::cerr << "Detalhes do erro: " << msg_err << std::endl;
    }

    // Reexibe a interface
    virtExibirInterface();
}

/// Desconecta do servidor.
/// Esta funcao soh pode ser chamada do programa principal,
/// jah que ela espera (join) pelo fim da thread do cliente.
void SupCliente::desconectar()
{
    // Cliente encerrado
    encerrarCliente = true;

    // Testa se estah conectado
    if (isConnected())
    {
        try
        {
            // Envia o comando de logout para o servidor
            /* ACRESCENTEI */
            uint16_t logoutCmd = CMD_LOGOUT;
            if (sock.write_uint16(logoutCmd) != mysocket_status::SOCK_OK)
            {
                std::cerr << "Erro: Falha ao enviar o comando de logout para o servidor." << std::endl;
            }
            // Espera 1 segundo para dar tempo ao servidor de ler a msg de LOGOUT
            // antes de fechar o socket
            /* ACRESCENTEI */
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            // Fecha o socket e, consequentemente, deve
            // encerrar a thread de leitura de dados do socket
            /* ACRESCENTEI */
            sock.close();
            std::cout << "Socket fechado com sucesso." << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Erro ao fechar o socket: " << e.what() << std::endl;
        }
    }

    // Aguarda fim da thread
    join_if_joinable();

    // Limpa o nome do usuario
    meuUsuario = "";
    // Limpa os dados armazenados de conexao anterior
    clearState();

    // Reexibe a interface
    virtExibirInterface();
}

/// Fixa o estado da valvula 1 (isV1==true) ou 2 (isV1==false)
/// como sendo aberta (Open==true) ou fechada (Open==false)
void SupCliente::setValvOpen(bool isV1, bool Open)
{
    // Comando enviado/recebido
    uint16_t cmd = (isV1 ? CMD_SET_V1 : CMD_SET_V2);

    // Bloqueia o mutex para garantir exclusao mutua no envio pelo socket
    // de comandos que ficam aguardando resposta, para evitar que a resposta
    // de um comando seja recebida por outro comando em outra thread.
    /* ACRESCENTEI */
    std::lock_guard<std::mutex> lock(mtx);

    try
    {
        // Testa se estah conectado e eh administrador
        if (!isConnected() || !isAdmin())
            throw 201;

        // Escreve o comando CMD_SET_V1 ou CMD_SET_V2
        // Em caso de erro, throw 202
        /* ACRESCENTEI */
        if (sock.write_uint16(cmd) != mysocket_status::SOCK_OK)
        {
            throw 202; // Erro ao enviar o comando
        }

        // Escreve o parametro do comando (==0 se fechada !=0 se aberta)
        // Em caso de erro, throw 203
        /* ACRESCENTEI */
        uint16_t state = (Open ? 1 : 0);
        if (sock.write_uint16(state) != mysocket_status::SOCK_OK)
        {
            throw 203; // Erro ao enviar o estado da v�lvula
        }

        // Leh a resposta (cmd) do servidor ao comando
        // Em caso de erro, throw 204
        /* ACRESCENTEI */
        if (sock.read_uint16(cmd) != mysocket_status::SOCK_OK)
        {
            throw 204; // Erro ao receber a resposta
        }

        // Se resposta nao for CMD_OK, throw 205
        if (cmd != CMD_OK)
        {
            throw 205; // Resposta inesperada do servidor
        }
    }
    catch (int err)
    {
        // Libera o mutex para sair da zona de exclusao mutua.
        /* ACRESCENTADAR */

        // Msg de erro para debug
        // Mensagem de erro para debug
        std::string msg_err = "Erro na operação da válvula ";
        msg_err += (isV1 ? "1: " : "2: ");
        msg_err += "Código do erro: " + std::to_string(err);
        virtExibirErro(msg_err);

        // Desconecta do servidor (reexibe a interface desconectada)
        desconectar();
    }

    // Libera o mutex para sair da zona de exclusao mutua.
    /* ACRESCENTEI */
    // O mutex é liberado automaticamente ao sair do escopo
    // devido ao uso de std::lock_guard, não é necessário liberar manualmente.

    // Nao reexibe a interface com novo estado.
    // Serah reexibida quando chegar o proximo dado do servidor, que
    // deverah conter o novo estado da valvula.
}

/// Fixa a entrada da bomba: 0 a 65535
void SupCliente::setPumpInput(uint16_t Input)
{
    // Comando recebido
    uint16_t cmd;
    // Msg de erro para debug
    std::string msg_err;

    // Bloqueia o mutex para garantir exclusao mutua no envio pelo socket
    // de comandos que ficam aguardando resposta, para evitar que a resposta
    // de um comando seja recebida por outro comando em outra thread.
    /* ACRESCENTEI */
    std::lock_guard<std::mutex> lock(mtx);

    try
    {
        // Testa se estah conectado e eh administrador
        if (!isConnected() || !isAdmin())
            throw 301;

        // Escreve o comando CMD_SET_PUMP
        // Em caso de erro, throw 302
        /* ACRESCENTEI */
        if (sock.write_uint16(cmd) != mysocket_status::SOCK_OK)
        {
            throw 302; // Erro ao enviar comando de configuração da bomba
        }

        // Escreve o paramentro do comando CMD_SET_PUMP (Input = 0 a 65535)
        // Em caso de erro, throw 303
        /* ACRESCENTEI */
        if (sock.write_uint16(Input) != mysocket_status::SOCK_OK)
        {
            throw 303; // Erro ao enviar o valor de entrada da bomba
        }

        // Leh a resposta do servidor ao comando
        // Em caso de erro, throw 304
        /* ACRESCENTAR */
        uint16_t response;
        if (sock.read_uint16(response) != mysocket_status::SOCK_OK)
        {
            throw 304; // Erro ao receber resposta do servidor
        }
        // Se resposta nao for CMD_OK, throw 305
        if (response != CMD_OK)
        {
            throw 305; // Resposta inesperada do servidor
        }
    }
    catch (int err)
    {
        // Libera o mutex para sair da zona de exclusao mutua.
        /* ACRESCENTAR */

        // Msg de erro para debug
        msg_err = "Erro na atuacao sobre a bomba: " + std::to_string(err);
        virtExibirErro(msg_err);

        // Desconecta do servidor (reexibe a interface desconectada)
        desconectar();
    }

    // Libera o mutex para sair da zona de exclusao mutua.
    /* ACRESCENTEI */
    // O mutex é liberado automaticamente ao sair do escopo
    // devido ao uso de std::lock_guard, não é necessário liberar manualmente.

    // Nao reexibe a interface com novo estado.
    // Serah reexibida quando chegar o proximo dado do servidor, que
    // deverah conter o novo estado da bomba.
}

/// Armazena o ultimo estado atual da planta.
/// Esta funcao pode ser complementada em uma interface especifica para
/// armazenar outros dados alem do ultimo estado da planta.
void SupCliente::storeState(const SupState &LastS)
{
    // Armazena o estado
    last_S = LastS;
    // Armazena o instante de recebimento de dados
    last_t = std::time(nullptr);
    // Inicializa o instante inicial, se for o primeiro ponto
    if (start_t == time_t(-1))
        start_t = last_t;
}

/// Limpa todos os estados armazenados da planta
/// Esta funcao pode ser complementada em uma interface especifica para
/// limpar outros dados alem do ultimo estado da planta.
void SupCliente::clearState()
{
    last_S = SupState();
    // Limpa o instante em que iniciou a coleta de dados
    // e o instante de leitura do ultimo estado.
    start_t = last_t = time_t(-1);
}

/// Thread de solicitacao periodica de dados
void SupCliente::main_thread(void)
{
    // Comando recebido
    uint16_t cmd;
    // Estado recebido
    SupState S;

    while (!encerrarCliente && isConnected())
    {

        // Bloqueia o mutex para garantir exclusao mutua no envio pelo socket
        // de comandos que ficam aguardando resposta, para evitar que a resposta
        // de um comando seja recebida por outro comando em outra thread.
        /* ACRESCENTADO */
        std::lock_guard<std::mutex> lock(mtx);

        try
        {
            // Escreve o comando CMD_GET_DATA
            // Em caso de erro, throw 401
            /* ACRESCENTADO */
            if (sock.write_uint16(cmd) != mysocket_status::SOCK_OK)
            {
                throw 401; // Erro ao enviar comando
            }

            // Leh a resposta do servidor ao pedido de dados (com timeout)
            // Em caso de erro, throw 402
            /* ACRESCENTADO */
            uint16_t response;

            // Define o timeout (em segundos ou milissegundos, dependendo da implementação)
            int timeout_ms = 5000; // Exemplo: timeout de 5000ms (5 segundos)

            // Leitura com timeout
            if (sock.read_uint16(response, timeout_ms) != mysocket_status::SOCK_OK) // Certifique-se de que `read_uint16` aceita timeout
            {
                std::cerr << "Erro: Tempo limite excedido ao esperar pela resposta do servidor para CMD_GET_DATA." << std::endl;
                throw 402; // Código de erro mantido
            }
            // Se resposta nao for CMD_OK, throw 403
            if (cmd != CMD_DATA)
            {
                throw 403;
            }

            // Leh os dados (com timeout)
            // Em caso de erro, throw 404
            /* ACRESCENTADO */
            if (sock.read_bytes(reinterpret_cast<mybyte *>(&S), sizeof(S), timeout_ms) != mysocket_status::SOCK_OK) // Supondo que read_bytes suporta timeout
            {
                std::cerr << "Erro: Tempo limite excedido ao receber os dados do servidor." << std::endl;
                throw 404; // Código de erro mantido
            }

            // Libera o mutex para sair da zona de exclusao mutua
            /* ACRESCENTEI */
            // O mutex será liberado automaticamente ao sair do escopo
            // devido ao uso de std::lock_guard. Não é necessário liberar manualmente.

            // Armazena os dados
            storeState(S);
            // Reexibe a interface
            virtExibirInterface();

            // Espera "timeRefresh" segundos
            /* ACRESCENTADO */
            std::this_thread::sleep_for(std::chrono::milliseconds(timeRefresh * 10));
        }
        catch (int err)
        {
            // Libera o mutex para sair da zona de exclusao mutua
            /* ACRESCENTADO */
            std::lock_guard<std::mutex> lock(mtx);

            // Nao pode chamar "desconectar" pq "desconectar" faz join na thread.
            // Como esta funcao main_thread eh executada na thread,
            // ela nao pode esperar pelo fim de si mesma.

            // Testa se estah conectado
            if (isConnected())
            {
                // Envia o comando de logout para o servidor
                /* ACRESCENTADO */
                uint16_t logoutCmd = CMD_LOGOUT;
                if (sock.write_uint16(logoutCmd) != mysocket_status::SOCK_OK)
                {
                    virtExibirErro("Erro ao enviar comando de logout para o servidor.");
                }
                // Espera 1 segundo para dar tempo ao servidor de ler a msg de LOGOUT
                // antes de fechar o socket
                /* ACRESCENTADO */
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                // Fecha o socket
                /* ACRESCENTADO */
                sock.close();
            }

            // Testa se o usuario desconectou na interface.
            // Se nao, emite msg de erro.
            if (!encerrarCliente)
            {
                // Msg de erro para debug
                std::string msg_err = std::string("Erro na requisicao de dados: ") + std::to_string(err);

                // Exibe msg de erro
                virtExibirErro(msg_err);
                std::cerr << "Erro detectado: " << msg_err << std::endl;

                // Reexibe a interface
                virtExibirInterface();
            }
        } // Fim do catch

    } // Fim do while (!encerrarCliente && isConnected())
}
