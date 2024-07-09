#include "classes.h"

std::string Manager::screen_format(const std::string &r1, const std::string &r2, const std::string &r3, const std::string &r4){
    std::ostringstream string_out;

    string_out << "\n\n";
    string_out << std::setw(10) << "\t" << "+----==================[" << r1 << "]===================----+";

    if(!r2.empty()){string_out << "\n" << r2 << std::endl;}
    if(!r3.empty() && !r2.empty()){string_out << r3 << std::endl;}

    if(r2.empty() && r3.empty()){string_out << "\n\n"; }
    string_out << "    " << r4;

    return string_out.str();
}

std::unordered_map<FS, std::string> flight_tags{
    {PLANNING, "planning"},
    {SUCCESSFUL, "successful"},
    {FAILED, "failure"},
    {IN_COURSE, "in course"}
};

// Astronauta class methods
Astronauta::Astronauta() : alive(true), available(true) {}

bool Astronauta::its_available() const { return available; }
bool Astronauta::its_alive() const { return alive; }
std::string Astronauta::get_cpf() const { return cpf; }
std::string Astronauta::get_name() const { return name; }
std::vector<std::string> Astronauta::get_flights() const { return flights; }
int Astronauta::get_age() const { return age; }

void Astronauta::toggle_available() { available = !available; }
void Astronauta::toggle_alive() { alive = !alive; }
void Astronauta::set_name(std::string new_name) { name = new_name; }
void Astronauta::set_cpf(std::string new_cpf) { cpf = new_cpf; }
void Astronauta::set_new_flight(std::string &new_flight) {
    flights.push_back(new_flight);
}
Astronauta::~Astronauta() {}

// Voo class methods
Voo::Voo() : state(PLANNING) {}

void Voo::set_code(int new_code) { code = new_code; }
int Voo::get_code() const { return code; }
std::vector<Astronauta> Voo::get_crew() const { return crew; }
FS Voo::get_state() const { return state; }

bool Voo::verify_crew() {
    if (crew.empty() || crew.size() < 1) { return false; }
    for (const auto &i : crew) {
        if (!i.its_available()) { return false; }
    }
    return true;
}

bool Voo::remove_member(std::string &cpf) {
    if (state != FS::PLANNING) { return false; }
    for (auto it = crew.begin(); it != crew.end(); ++it) {
        if (it->get_cpf() == cpf) {
            it->toggle_available();
            crew.erase(it);
            return true;
        }
    }
    return false;
}

bool Voo::add_member(Astronauta &astronauta) {
    if (state != FS::PLANNING) { return false; }
    if (!astronauta.its_available()) { return false; }
    astronauta.toggle_available();
    crew.push_back(astronauta);
    return true;
}

bool Voo::explode_flight() {
    if (state != FS::IN_COURSE) { return false; }
    std::ostringstream flight_data;
    flight_data << "F_ID: " << code << " | R: F";
    std::string flight_report = flight_data.str();
    for (auto &i : crew) { 
        i.toggle_alive(); 
        i.set_new_flight(flight_report); 
    }
    state = FS::FAILED;
    return true;
} 

bool Voo::end_flight() {
    if (state != FS::IN_COURSE) { return false; }
    std::ostringstream flight_data;
    flight_data << "F_ID: " << code << " | R: S";
    std::string flight_report = flight_data.str();
    for (auto &i : crew) { 
        i.toggle_available(); 
        i.set_new_flight(flight_report); 
    }
    state = SUCCESSFUL;
    return true;
}

bool Voo::send_flight() {
    if (state != FS::PLANNING || !verify_crew()) { return false; }
    state = FS::IN_COURSE;
    return true;
}

Voo::~Voo() {}

// Manager class methods

void Manager::cadastrar_astronauta(const std::string& name, const std::string& cpf, int age) {
    Astronauta new_astronauta;
    new_astronauta.set_name(name);
    new_astronauta.set_cpf(cpf);
    astronautas_operacoes[cpf] = new_astronauta;
}

void Manager::cadastrar_voo() {
    Voo new_voo;
    int code = ++auto_id_flight;
    new_voo.set_code(code);
    voos_operacoes[std::to_string(code)] = new_voo;
    voos_leitura.push_back(new_voo);
}

void Manager::listar_voos() {
    if (voos_leitura.empty()) {
        std::cout << "\t >>> Não há voos para exibir.\n";
        return;
    }
    for (const auto& voo_pair : voos_operacoes) {
        const Voo& voo = voo_pair.second;
        std::cout << "Flight Code: " << voo.get_code() << " | State: " << flight_tags[voo.get_state()] << "\n";
        std::vector<Astronauta> leitura = voo.get_crew();
        for (const auto& i : leitura) {
            std::cout << i.get_cpf() << std::endl;
        }
    }
}

void Manager::listar_astronautas(){
    for (const auto& astronauta : astronautas) {
        std::cout << "Name: " << astronauta.get_name() << " | CPF: " << astronauta.get_cpf() << "\nMissões:";
    }
}

void Manager::listar_mortos() {
    if (astronautas.empty()) {
        std::cout << "\t >>> Não há astronautas.\n";
        return;
    }
    for (const auto& astronauta : astronautas) {
        if(!astronauta.its_alive()){
            std::cout << "Name: " << astronauta.get_name() << " | CPF: " << astronauta.get_cpf() << "\nMissões:";
            std::vector<std::string> leitura = astronauta.get_flights();
            for (const auto& i : leitura) {
                std::cout << i << std::endl;
            }
        }
    }
}

void Manager::process() {
    if (state == MS::MAIN_MENU) {
        std::cin >> option;
    }
    else if (state == MS::REGIST_T) {
        std::cin >> r_type;
    }
    else if (state == MS::LIST_T) {
        std::cin >> l_type;
    }
    else if (state == MS::ACT_T) {
        std::cin >> act_type;
        std::cin >> id_voo_op;
    }
    else if (state == MS::GM_T) {
        std::cin >> gm_type;
        std::cin >> id_voo_op;
    }
    else if (state == MS::REGIST) {
        if (r_type == 1) {
            std::string name, cpf;
            int age;
            std::cin >> name >> cpf >> age;
            if (astronautas_operacoes.find(cpf) != astronautas_operacoes.end()) {
                error = MSG::INV_AS;
            } else {
                cadastrar_astronauta(name, cpf, age);
            }
        } else {
            cadastrar_voo();
        }
    }
    else if (state == MS::ACT) {
        std::string id = id_voo_op;
        if (act_type == 1) { // Mandar
            if (voos_operacoes.find(id) != voos_operacoes.end()) {
                voos_operacoes[id].send_flight();
            } else {
                error = MSG::ERR_SEND;
            }
        }
        else if (act_type == 2) { // Finalizar
            if (voos_operacoes.find(id) != voos_operacoes.end()) {
                voos_operacoes[id].end_flight();
            } else {
                error = MSG::ERR_FIN;
            }
        }
        else { // Explodir
            if (voos_operacoes.find(id) != voos_operacoes.end()) {
                voos_operacoes[id].explode_flight();
            } else {
                error = MSG::ERR_REM;
            }
        }
    }
    else if (state == MS::GM) {
        std::string id = id_voo_op, cpf_rm;
        std::cin >> cpf_rm;
        if (gm_type == 1) {
            if (voos_operacoes.find(id) != voos_operacoes.end()) {
                if (astronautas_operacoes.find(cpf_rm) != astronautas_operacoes.end()) {
                    voos_operacoes[id].remove_member(cpf_rm);
                } else {
                    error = MSG::ERR_REM;
                }
            } else {
                error = MSG::ERR_REM;
            }
        }
        else {
            if (voos_operacoes.find(id) != voos_operacoes.end()) {
                if (astronautas_operacoes.find(cpf_rm) != astronautas_operacoes.end()) {
                    voos_operacoes[id].add_member(astronautas_operacoes[cpf_rm]);
                } else {
                    error = MSG::ERR_ADD;
                }
            } else {
                error = MSG::ERR_ADD;
            }
        }
    }
}

void Manager::update() {
    if (state == MS::MAIN_MENU) {
        if (option >= 1 && option <= 4) {
            if (option == 1) state = MS::REGIST_T;
            else if (option == 2) state = MS::LIST_T;
            else if (option == 3) state = MS::ACT_T;
            else if (option == 4) state = MS::GM_T;
        } else {
            error = MSG::INV_IPT;
        }
    }
    else if (state == MS::REGIST_T) {
        if (r_type == 1 || r_type == 2) state = MS::REGIST;
        else error = MSG::INV_IPT;
    }
    else if (state == MS::LIST_T) {
        if (l_type >= 1 && l_type <= 4) state = MS::LIST;
        else error = MSG::INV_IPT;
    }
    else if (state == MS::ACT_T) {
        if (act_type >= 1 && act_type <= 3) state = MS::ACT;
        else error = MSG::INV_IPT;
    }
    else if (state == MS::GM_T) {
        if (gm_type == 1 || gm_type == 2) state = MS::GM;
        else error = MSG::INV_IPT;
    }
    else if (state == MS::REGIST || state == MS::ACT || state == MS::GM || state == MS::LIST) {
        if (exit != -1) {
            exit = -1;
            state = MS::MAIN_MENU;
        }
    }
}

void Manager::render() {
    if (state == MS::MAIN_MENU) {
        std::cout << screen_format("Menu Principal", "1. Registrar\n2. Listar\n3. Ações de voo\n4. Gerenciar membros\n", "", "Insira uma opção > ");
    }
    else if (state == MS::REGIST_T) {
        std::cout << screen_format("Aba de Registro", "1. Registrar Astronauta\n2. Registrar Voo\n", "", "Insira uma opção > ");
    }
    else if (state == MS::ACT_T) {
        std::cout << screen_format("Aba de Ações de Voo", "1. Mandar Voo\n2. Finalizar Voo\n3. Explodir Voo\n", "", "Insira uma opção > ");
    }
    else if (state == MS::LIST_T) {
        std::cout << screen_format("Aba de Listagens", "1. Listar Astronautas\n2. Listar Voos\n3. Listar Voos em Plano\n4. Listar Voos em Curso\n", "", "Insira uma opção > ");
    }
    else if (state == MS::GM_T) {
        std::cout << screen_format("Aba de Adicionar/Remover", "1. Remover Membro\n2. Adicionar Membro\n", "", "Insira uma opção > ");
    }
    else if (state == MS::REGIST) {
        if (r_type == 1) {
            std::cout << screen_format("Registrar Astronauta", "", "", "Insira Nome, CPF e Idade > ");
        } else {
            std::cout << screen_format("Registrar Voo", "", "", "Registrar voo iniciado...");
        }
    }
    else if (state == MS::ACT) {
        if (act_type == 1) {
            std::cout << screen_format("Mandar Voo", "", "", "Mandar voo iniciado...");
        } else if (act_type == 2) {
            std::cout << screen_format("Finalizar Voo", "", "", "Finalizar voo iniciado...");
        } else {
            std::cout << screen_format("Explodir Voo", "", "", "Explodir voo iniciado...");
        }
    }
    else if (state == MS::GM) {
        if (gm_type == 1) {
            std::cout << screen_format("Remover Membro", "", "", "Insira CPF do membro a remover > ");
        } else {
            std::cout << screen_format("Adicionar Membro", "", "", "Insira CPF do membro a adicionar > ");
        }
    }
}

