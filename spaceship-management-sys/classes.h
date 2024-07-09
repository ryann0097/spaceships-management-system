#ifndef CLASSES_H_
#define CLASSES_H_

#include <iostream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <iomanip>

enum FS {
    PLANNING,
    SUCCESSFUL,
    FAILED,
    IN_COURSE
};

enum MS {
    START,
    MAIN_MENU,
    REGIST,
    REGIST_T, // < TIPO DE REGISTRO A SER REALIZADO
    REGIST,
    LIST_T,
    LIST,
    ACT_T,
    ACT,
    GM_T,
    GM,
    END
};

enum MSG{
    NONE, // default
    INV_IPT, // ENTRADA INVALIDA DE OPÇÃO
    INV_AS, // ASTRONAUTA EXISTENTE (CPF)
    ERR_ADD, // VOO NAO ESTA EM PLAN OU AUSTRONAUT N TA
    ERR_REM, // VOO NAO ESTA EM PLAN OU ASTRONAUTA NAO DISP
    ERR_FIN, // VOO NAO ESTA EM CURSO
    ERR_SEND, // VOO NAO ESTÁ EM CURSO OU NAO TEM MINIMO DE PASSAGEIRO
};

extern std::unordered_map<FS, std::string> flight_tags;
extern std::unordered_map<MSG, std::string> msgs;

class Astronauta {
    private:
    std::string name; 
    std::string cpf; 
    int age; 
    bool alive; 
    bool available; 
    std::vector<std::string> flights;

    public:
    Astronauta();
    bool its_available() const;
    bool its_alive() const;
    std::string get_cpf() const;
    std::string get_name() const;
    std::vector<std::string> get_flights() const;
    int get_age() const;
    void toggle_available();
    void toggle_alive();
    void set_name(std::string new_name);
    void set_cpf(std::string new_cpf);
    void set_new_flight(std::string &new_flight);
    ~Astronauta();
};

class Voo {
    private:
    int code;
    std::vector<Astronauta>crew;
    FS state;

    public:
    Voo();
    void set_code(int new_code);
    int get_code() const;
    std::vector<Astronauta> get_crew() const;
    FS get_state() const;
    bool verify_crew();
    bool remove_member(std::string &cpf); // astronauta nao ta no voo ou o não é permitido alterar
    bool add_member(Astronauta &astronauta); // astronauta nao disponivel ou nao permitido alterar
    bool explode_flight(); // nao esta em voo
    bool end_flight(); // nao esta em voo
    bool send_flight(); // retorna falso caso nao de 
    ~Voo();
};

class Manager {
    private:
    MS state = START;
    MSG error = NONE;
    int auto_id_flight = 0;
    int option = -1;
    int exit = -1;

    std::string id_voo_op = "";

    std::unordered_map<std::string, Astronauta> astronautas_operacoes;
    std::unordered_map<std::string, Voo> voos_operacoes;
    std::vector<Astronauta> astronautas;
    std::vector<Voo> voos_leitura;

    int r_type = -1; // para registros
    int l_type = -1; // para listagens
    int act_type = -1; // para voos
    int gm_type = -1; // para retirar ou add astronautas

    public:
    Manager();
    void cadastrar_astronauta(const std::string& name, const std::string& cpf, int age);
    void cadastrar_voo();
    void listar_voos();
    void listar_mortos();
    void listar_astronautas();

    void process();
    void update();
    void render();
    std::string screen_format(const std::string &r1, const std::string &r2, const std::string &r3, const std::string &r4);
    bool fim(){return state == MS::END;}
};

#endif
