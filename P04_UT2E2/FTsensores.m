% Cargar datos desde el archivo CSV
data = readtable('datos_bp805.csv');

% Extraer variables
tiempo = data{:,1};          % tiempo en minutos
temperatura = data{:,2};     % temperatura en °C

% Definir el tiempo de muestreo (en minutos)
Ts = tiempo(2) - tiempo(1);  % asume muestreo uniforme

% Crear la señal de entrada (escalón de 3.3 V desde el inicio)
entrada = 3.3 * ones(size(tiempo));

% Crear objeto iddata para System Identification
datos_id = iddata(temperatura, entrada, Ts);

% Estimar modelo de primer orden (1 polo, 0 ceros)
modelo_1p = tfest(datos_id, 1, 0);

% Mostrar la función de transferencia identificada
disp('Función de transferencia estimada:');
modelo_1p

% Graficar comparación entre modelo y datos reales
figure;
compare(datos_id, modelo_1p);
title('Comparación: Modelo vs Datos reales');
plot(modelo_1p);
title('Comparación: Modelo');
