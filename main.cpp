#include <iostream>
#include <unordered_map>
#include <memory>
#include <vector>
#include <cstdlib> // Para rand() e srand()
#include <ctime>   // Para time()
#include <thread>  // Para std::thread
#include <mutex>   // Para std::mutex
#include <queue>   // Para std::queue
#include <condition_variable>

// Classe abstrata Operacao
class Operacao {
public:
    virtual int calcular(int operador1, int operador2) const = 0; // Método abstrato
    virtual ~Operacao() = default;  // Destrutor virtual
};

// Classe Soma que implementa Operacao
class Soma : public Operacao {
public:
    int calcular(int operador1, int operador2) const override {
        return operador1 + operador2;
    }
};

// Classe Subtracao que implementa Operacao
class Subtracao : public Operacao {
public:
    int calcular(int operador1, int operador2) const override {
        return operador1 - operador2;
    }
};

// Classe Multiplicacao que implementa Operacao
class Multiplicacao : public Operacao {
public:
    int calcular(int operador1, int operador2) const override {
        return operador1 * operador2;
    }
};

// Classe Divisao que implementa Operacao
class Divisao : public Operacao {
public:
    int calcular(int operador1, int operador2) const override {
        if (operador2 == 0) {
            std::cerr << "Erro: Divisao por zero!" << std::endl;
            return 0;
        }
        return operador1 / operador2;
    }
};

// Estruturas para sincronização de threads
std::mutex mtx;
std::condition_variable cv;
std::queue<int> queue;

// Função que realiza o cálculo e imprime o resultado
void realizarCalculo(int index, int num1, int num2, char operador, const std::unordered_map<char, std::shared_ptr<Operacao>>& operacoes) {
    // Lock para evitar condições de corrida ao acessar o mapa de operações
    std::unique_lock<std::mutex> lock(mtx);

    if (operacoes.find(operador) != operacoes.end()) {
        // Chamar o método calcular da operação correspondente
        int resultado = operacoes.at(operador)->calcular(num1, num2);
        std::cout << "Thread #" << std::this_thread::get_id() << " - Operacao #" << index << ": " << num1 << " " << operador << " " << num2 << " = " << resultado << std::endl;
    } else {
        std::cerr << "Operacao invalida!" << std::endl;
    }

    // Marcar como concluído e notificar outras threads
    queue.pop();
    cv.notify_all();
}

// Função principal
int main() {
    // Inicializar gerador de números aleatórios
    srand(static_cast<unsigned int>(time(0)));

    // Criar um mapa associando os operadores a instâncias das classes
    std::unordered_map<char, std::shared_ptr<Operacao>> operacoes;
    operacoes['+'] = std::make_shared<Soma>();
    operacoes['-'] = std::make_shared<Subtracao>();
    operacoes['*'] = std::make_shared<Multiplicacao>();
    operacoes['/'] = std::make_shared<Divisao>();

    // Criar um array com 20 operações aleatórias
    std::vector<std::pair<int, char>> operacoesArray;
    char operadores[] = {'+', '-', '*', '/'};

    for (int i = 0; i < 20; ++i) {
        int num1 = rand() % 100;  // Gera um número aleatório entre 0 e 99
        char operador = operadores[rand() % 4];  // Escolhe aleatoriamente entre +, -, *, /
        operacoesArray.push_back({num1, operador});
    }

    // Vetor de threads
    std::vector<std::thread> threads;

    // Loop para executar os cálculos com no máximo 5 threads em paralelo
    for (int i = 0; i < operacoesArray.size(); ++i) {
        int num1 = operacoesArray[i].first;
        int num2 = rand() % 100;  // Gera um número aleatório entre 0 e 99 para o segundo operador
        char operador = operacoesArray[i].second;

        // Sincronizar o número de threads ativas
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [] { return queue.size() < 5; });  // Espera até ter menos de 5 operações
            queue.push(i);  // Adiciona a operação na fila
        }

        // Criar e iniciar a thread
        threads.emplace_back(realizarCalculo, i, num1, num2, operador, std::ref(operacoes));
    }

    // Esperar todas as threads terminarem
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    return 0;
}