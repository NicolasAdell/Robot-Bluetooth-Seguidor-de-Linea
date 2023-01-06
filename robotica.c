#include <stdio.h>
#include "pico/stdlib.h"

// Prototipos de las funciones
void ultrasonico_init(uint8_t trigger, uint8_t echo);
float ultrasonico_get_distance_cm(uint8_t trigger, uint8_t echo);
void motor_init(uint8_t mot_ade, uint8_t mot_atr);
void motor_forward(uint8_t mot_ade, uint8_t mot_atr);
void motor_reverse(uint8_t mot_ade, uint8_t mot_atr);
void motor_stop(uint8_t mot_ade, uint8_t mot_atr); 
void infrarrojo_init(uint8_t inf);
bool infrarrojo_get(uint8_t inf);
void bluetooth_init(uint8_t tx, uint8_t rx, uint8_t mode);
bool bluetooth_mode_get(uint8_t mode);

int main(void)
{   
    // Establece los GPIOs como tx y rx, para la posterior utilizacón del UART
    const uint8_t TX = 0;
    const uint8_t RX = 1;

    // Establece los GPIOs para los infrarrojos
    const uint8_t INF_FREN = 6;
    const uint8_t INF_DER = 7;
    const uint8_t INF_IZQ = 8;

    // Establece el GPIO para el pin de modo
    const uint8_t BLUETOOTH_MODE = 11;

    // Establece los GPIOs para el ultrasónico
    const uint8_t TRIGGER = 12; //establece el trigger en el GPIO12
    const uint8_t ECHO = 13; //establece el echo en el GPIO13

    // Establece los GPIOs para los motores
    const uint8_t MOT_DER_ADE = 14;
    const uint8_t MOT_DER_ATR = 15;
    const uint8_t MOT_IZQ_ATR = 16;
    const uint8_t MOT_IZQ_ADE = 17;

    /* Inicialización de los componentes */

    stdio_init_all();

    ultrasonico_init(TRIGGER, ECHO);

    infrarrojo_init(INF_FREN);
    infrarrojo_init(INF_DER);
    infrarrojo_init(INF_IZQ);

    motor_init(MOT_DER_ADE, MOT_DER_ATR);
    motor_init(MOT_IZQ_ADE, MOT_IZQ_ATR);
    
    bluetooth_init(TX, RX, BLUETOOTH_MODE);

    // Condición inicial del cotador
    int count = 0;

    while (true)
    {   
        // Calcula distancia del infrarrojo
        float distance = ultrasonico_get_distance_cm(TRIGGER, ECHO); //el ultrasónico calcula la distancia

        // Se obtiene el valor de los infrarrojos
        bool infrarrojo_fren = infrarrojo_get(INF_FREN); //el infrarrojo del frente da su valor
        bool infrarrojo_der = infrarrojo_get(INF_DER); //el infrarrojo de la derecha da su valor
        bool infrarrojo_izq = infrarrojo_get(INF_IZQ); //el infrarrojo de la izquierda da su valor

        // Verifica el estado de la llave de modo bluettoth
        bool bluetooth_mode = bluetooth_mode_get(BLUETOOTH_MODE);

        // Modo bluetooth
        if (bluetooth_mode)
        {  

            // Al pasar a modo bluetooth, el contador vuelve a cero
            count = 0;

            // Verifica que los datos en el UART se puedan leer
            if (uart_is_readable(uart0))
            {   
                // Obtiene el comando
                char command = uart_getc(uart0);

                // Si el comando es una 'F', el robot debe avanzar
                if (command == 'F')
                { 
                    motor_forward(MOT_DER_ADE, MOT_DER_ATR);
                    motor_forward(MOT_IZQ_ADE, MOT_IZQ_ATR);
                }
                
                // Si el comando es una 'B', el robot debe retroceder
                else if (command == 'B')
                { 
                    motor_reverse(MOT_DER_ADE, MOT_DER_ATR);
                    motor_reverse(MOT_IZQ_ADE, MOT_IZQ_ATR);
                }

                // Si el comando es una 'R', el robot debe girar a la derecha
                else if (command == 'R')
                { 
                    motor_forward(MOT_IZQ_ADE, MOT_IZQ_ATR);
                    motor_reverse(MOT_DER_ADE, MOT_DER_ATR);
                }

                // Si el comando es una 'L', el robot debe girar a la izquierda
                else if (command == 'L')
                {
                    motor_forward(MOT_DER_ADE, MOT_DER_ATR); //avanza el motor derecho
                    motor_reverse(MOT_IZQ_ADE, MOT_IZQ_ATR); //retrocede el motor izquierdo
                }

                // Si el comando es una 'S', el robot debe frenar
                else if (command == 'S')
                {
                    motor_stop(MOT_IZQ_ADE, MOT_IZQ_ATR);
                    motor_stop(MOT_DER_ADE, MOT_DER_ATR);
                }

                // Si el comando es una 'G', el robot debe girar levemente a la izquierda
                else if (command == 'G')
                {
                    motor_forward(MOT_DER_ADE, MOT_DER_ATR);
                    motor_stop(MOT_IZQ_ADE, MOT_IZQ_ATR);
                }

                // Si el comando es una 'I', el robot debe girar levemente a la derecha
                else if (command == 'I')
                {
                    motor_forward(MOT_IZQ_ADE, MOT_IZQ_ATR);
                    motor_stop(MOT_DER_ADE, MOT_DER_ATR);
                }

                // Si el comando es una 'I', el robot debe retroceder levemente a la derecha
                else if (command == 'H')
                {
                    motor_reverse(MOT_DER_ADE, MOT_DER_ATR);
                    motor_stop(MOT_IZQ_ADE, MOT_IZQ_ATR);
                }

                // Si el comando es una 'J', el robot debe retroceder levemente a la derecha
                else if (command == 'J') //condición que se mande una J para que el robot retroceda hacia la derecha levemente
                {
                    motor_reverse(MOT_IZQ_ADE, MOT_IZQ_ATR);
                    motor_stop(MOT_DER_ADE, MOT_DER_ATR);
                }

                // Si no hay comando, por default se para el robot
                else
                {
                    motor_stop(MOT_IZQ_ADE, MOT_IZQ_ATR);
                    motor_stop(MOT_DER_ADE, MOT_DER_ATR);
                }
            }

            // Si el UART no es legible, el robot se frena
            else
            {
                motor_stop(MOT_IZQ_ADE, MOT_IZQ_ATR); //se frena el motor izquierdo
                motor_stop(MOT_DER_ADE, MOT_DER_ATR); //se frena el motor derecho
            }
        }

        // Modo automático
        else
        {   
            // Si hay un objeto a 75 cm o menos, el robot se para
            if (distance <= 75)
            {
                count = 0;
                motor_stop(MOT_DER_ADE, MOT_DER_ATR);
                motor_stop(MOT_IZQ_ADE, MOT_IZQ_ATR);
            }

            // Si no hay objeto, se guía por infrarrojos
            else
            {
                
                // El robot avanza
                if (infrarrojo_fren == 1 && infrarrojo_der == 1 && infrarrojo_izq == 1) //condición para que el robot avance 
                {   
                    count = 0;
                    motor_forward(MOT_DER_ADE, MOT_DER_ATR); //avanza el motor derecho
                    motor_forward(MOT_IZQ_ADE, MOT_IZQ_ATR); //avanza el motor izquierdo
                }

                // El robot avanza
                else if (infrarrojo_fren == 1 && infrarrojo_der == 0 && infrarrojo_izq == 0) // condición para que el robot avance
                {   
                    count = 0;
                    motor_forward(MOT_DER_ADE, MOT_DER_ATR);
                    motor_forward(MOT_IZQ_ADE, MOT_IZQ_ATR);
                }   

                // El robot gira levementa hacia la derecha
                else if (infrarrojo_fren == 1 && infrarrojo_der == 1 && infrarrojo_izq == 0)
                {
                    count = 0;
                    motor_forward(MOT_IZQ_ADE, MOT_IZQ_ATR);
                    motor_stop(MOT_DER_ADE, MOT_DER_ATR);
                }

                // El robot gira levementa hacia la izquierda
                else if (infrarrojo_fren == 1 && infrarrojo_der == 0 && infrarrojo_izq == 1)
                {
                    count = 0;
                    motor_forward(MOT_DER_ADE, MOT_DER_ATR);
                    motor_stop(MOT_IZQ_ADE, MOT_IZQ_ATR);
                }

                // El robot gira hacia la derecha
                else if (infrarrojo_fren == 0 && infrarrojo_der == 1  && infrarrojo_izq == 0) //condición para que el robot gire a la derecha
                {
                    count = 0;
                    motor_forward(MOT_IZQ_ADE, MOT_IZQ_ATR);
                    motor_reverse(MOT_DER_ADE, MOT_DER_ATR);
                }   

                // El robot gira hacia la izquierda
                else if (infrarrojo_fren == 0 && infrarrojo_der == 0  && infrarrojo_izq == 1) //condición para que el robot gire a la izquierda
                {
                    count = 0;
                    motor_forward(MOT_DER_ADE, MOT_DER_ATR); //avanza el motor derecho
                    motor_reverse(MOT_IZQ_ADE, MOT_IZQ_ATR); //retrocede el motor izquierdo
                }

                else
                {
                    absolute_time_t t0;

                    // Verifica si es la primera vez que pasa por el condicional
                    if (count == 0)
                    {   
                        // Obtiene tiempo inicial
                        t0 = get_absolute_time();
                    }

                    // El robot gira hacia la izquierda
                    motor_forward(MOT_DER_ADE, MOT_DER_ATR);
                    motor_reverse(MOT_IZQ_ADE, MOT_IZQ_ATR);
                    
                    // Calculo instante actual
                    absolute_time_t t1 = get_absolute_time();

                    // El robot retrocede si giró dos segundos
                    if(absolute_time_diff_us(t0, t1)/1e6 >= 2.0) //condición por si se queda en este apartado por 3 s o más
                    { 
                        motor_reverse(MOT_DER_ADE, MOT_DER_ATR);
                        motor_reverse(MOT_IZQ_ADE, MOT_IZQ_ATR);
                    }
                    
                    // El robot se frena si siguió girando hasta los 4 segundos
                    else if(absolute_time_diff_us(t0, t1)/1e6 >= 4.0)
                    { 
                        motor_stop(MOT_DER_ADE, MOT_DER_ATR);
                        motor_stop(MOT_IZQ_ADE, MOT_IZQ_ATR);
                    }
                    
                    count++;
                }
            }
        }

    }
    return 0;
}

// Inicialización del ultrasónico
void ultrasonico_init(uint8_t trigger, uint8_t echo)
{
    // Se setea el trigger como salida
    gpio_init(trigger);
    gpio_set_dir(trigger, GPIO_OUT);

    // Se setea el echo como entrada
    gpio_init(echo);
    gpio_set_dir(echo, GPIO_IN);
}

// Cálculo del ultrasónico de la distancia
float ultrasonico_get_distance_cm(uint8_t trigger, uint8_t echo)
{
    // Se envía el pulso durante 10 us
    gpio_put(trigger, true);
    sleep_us(10);
    gpio_put(trigger, false);

	absolute_time_t from, to;

    // Cuando el echo está en cero, se va guardando los instantes hasta que emiece a recibir
    while (!gpio_get(echo))
    {
        from = get_absolute_time();
    }

    // Cuando el echo está en uno, se va guardando los instantes hasta que deje de recibir
    while (gpio_get(echo)) 
    {
        to = get_absolute_time(); //instante en que recibe el 1 del trigger
    }

    // Se calcula anche de puslo y se devuelve la distancia
    int64_t pulse_width = absolute_time_diff_us(from, to);
    return pulse_width / 58.28;
}

// Inicialización de los motores
void motor_init(uint8_t mot_ade, uint8_t mot_atr)
{   
    // Se establece el pin del motor utilizado para avanzar como salida
    gpio_init(mot_ade);
    gpio_set_dir(mot_ade, GPIO_OUT);

    // Se establece el pin del motor utilizado para retroceder como salida
    gpio_init(mot_atr);
    gpio_set_dir(mot_atr, GPIO_OUT);
}

// Función para que un motor avance
void motor_forward(uint8_t mot_ade, uint8_t mot_atr)
{   
    gpio_put(mot_ade, true);
    gpio_put(mot_atr, false);
}

// Función para que un motor retroceda
void motor_reverse(uint8_t mot_ade, uint8_t mot_atr)
{
    gpio_put(mot_ade, false);
    gpio_put(mot_atr, true);
}

// Función para que un motor se pare
void motor_stop(uint8_t mot_ade, uint8_t mot_atr)
{
    gpio_put(mot_ade, false);
    gpio_put(mot_atr, false);
}

// Función que inicializa el infrarrojo
void infrarrojo_init(uint8_t inf)
{   
    // Establece el pin de infrarrojo como entrada
    gpio_init(inf);
    gpio_set_dir(inf, GPIO_IN);
}

// Fución que obtiene el valor del infrarrojo
bool infrarrojo_get(uint8_t inf)
{
    // El valor se niega para que el blanco sea un uno y el negro un cero
    bool infrarrojo = !gpio_get(inf);
    return infrarrojo;
}

// Función que inicializa el bluetooth
void bluetooth_init(uint8_t tx, uint8_t rx, uint8_t mode)
{
    // Se inicializa el UART
    uart_init(uart0, 9600);

    // Se establecen los pines del UART
    gpio_set_function(tx, GPIO_FUNC_UART);
    gpio_set_function(rx, GPIO_FUNC_UART);

    // Se setea el pin de modo como entrada
    gpio_init(mode);
    gpio_set_dir(mode, GPIO_IN);

    // Se habiliita la conexión fifo para leer el UART
    uart_set_fifo_enabled(uart0, true);
}

// Función que obtiene el estado del pin de modo
bool bluetooth_mode_get(uint8_t mode)
{
    bool bluetooth_mode = gpio_get(mode);
    return bluetooth_mode;
}

