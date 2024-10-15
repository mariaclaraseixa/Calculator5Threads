Este projeto implementa uma calculadora multithreaded em C++ que realiza 20 operações aritméticas escolhidas de forma aleatória,
sendo processadas por no máximo 5 threads em paralelo. O código usa herança, polimorfismo, std::unordered_map, e
sincronização de threads para realizar as operações aritméticas com segurança e eficiência.

Funcionalidades

	•	Suporte para as operações: +, -, *, /
	•	Utilização de multithreading para executar múltiplas operações simultaneamente
	•	Sincronização de threads utilizando mutex e condition variable
	•	Geração de números e operações aritméticas de forma aleatória
	•	Verificação de divisão por zero

Estrutura do Código

	•	Classe base Operacao: Define o método abstrato calcular() implementado pelas subclasses.
	•	Subclasses: Soma, Subtracao, Multiplicacao, Divisao implementam o método calcular() correspondente.
	•	std::unordered_map: Mapeia operadores (+, -, *, /) para suas respectivas operações.
	•	Multithreading: Utiliza std::thread para realizar cálculos em paralelo, com no máximo 5 threads ativas simultaneamente. std::mutex e std::condition_variable são usados para sincronização.
	•	Fila de operações: Utiliza std::queue para controlar o número de operações ativas, limitando o processamento simultâneo a 5 operações.
