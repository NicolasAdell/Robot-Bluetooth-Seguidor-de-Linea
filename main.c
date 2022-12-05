#include <stdio.h>
#include "pico/stdlib.h"

void ultrasonico_init(uint8_t trigger, uint8_t echo); //función para inicialiar ultrasonico
float ultrasonico_get_distance_cm(uint8_t trigger, uint8_t echo); //función que usa el ultrasónico para la distancia
void motor_init(uint8_t mot_ade, uint8_t mot_atr); //función para inicializar los motores
void motor_forward(uint8_t mot_ade, uint8_t mot_atr); //función que hace el motor avanzar
void motor_reverse(uint8_t mot_ade, uint8_t mot_atr); //función que hace el motor retroceder
void motor_stop(uint8_t mot_ade, uint8_t mot_atr); //función que hace parar el motor
void infrarrojo_init(uint8_t inf); //función que inicializa el innfrarrojo
bool infrarrojo_get(uint8_t inf); //función que devuelve el valor del infrarrojo
void bluetooth_init(uint8_t tx, uint8_t rx, uint8_t mode); //función que inicializa el modulo bluetooth
bool bluetooth_mode_get(uint8_t mode); //función que obtiene el valor de pin de modo

int main(void) //programa principal
{   
    const uint8_t TX = 0; //establece tx como GPIO0
    const uint8_t RX = 1; //establece rx como GPIO1

    const uint8_t INF_FREN = 6; //establece el infrarrojo de enfrente en el GPIO6
    const uint8_t INF_DER = 7; //establece el infrarrojo de la derecha en el GPIO7
    const uint8_t INF_IZQ = 8; //establece el infrarrojo de la izquierda en el GPIO8

    const uint8_t BLUETOOTH_MODE = 11; //establece el pin de modo en el GPIO11

    const uint8_t TRIGGER = 12; //establece el trigger en el GPIO12
    const uint8_t ECHO = 13; //establece el echo en el GPIO13

    const uint8_t MOT_DER_ADE = 14; //establece el motor de la derecha hacia adelante en el GPIO14
    const uint8_t MOT_DER_ATR = 15; //establece el motor de la derecha hacia atrás en el GPIO15
    const uint8_t MOT_IZQ_ATR = 16; //establece el motor de la izquierda hacia adelante en el GPIO16
    const uint8_t MOT_IZQ_ADE = 17; //establece el motor de la izquierda hacia atrás en el GPIO17

    stdio_init_all(); //se inicializa todo

    ultrasonico_init(TRIGGER, ECHO); //se inicializa el ultrasónico

    infrarrojo_init(INF_FREN); //se inicializa el infrarrojo del frente
    infrarrojo_init(INF_DER); //se inicializa el infrarrojo de la derecha
    infrarrojo_init(INF_IZQ); //se inicializa el infrarrojo de la izquierda

    motor_init(MOT_DER_ADE, MOT_DER_ATR); //se inicializa el motor de la derecha
    motor_init(MOT_IZQ_ADE, MOT_IZQ_ATR); //se inicializa el motor de la izquierda
    
    bluetooth_init(TX, RX, BLUETOOTH_MODE); //se inicializa el bluetooth

    int i = 0; //condición inicial para un contador

    while (true) //programa en loop
    { 
        float distance = ultrasonico_get_distance_cm(TRIGGER, ECHO); //el ultrasónico calcula la distancia

        bool infrarrojo_fren = infrarrojo_get(INF_FREN); //el infrarrojo del frente da su valor
        bool infrarrojo_der = !infrarrojo_get(INF_DER); //el infrarrojo de la derecha da su valor
        bool infrarrojo_izq = !infrarrojo_get(INF_IZQ); //el infrarrojo de la izquierda da su valor

        bool bluetooth_mode = bluetooth_mode_get(BLUETOOTH_MODE); //el pin de modo da su valor

        if (bluetooth_mode) //condición que el pin de modo esté en 1 para el uso del modo bluetooth
        {
            if (uart_is_readable(uart0)) //condición que el uart0 sea legible para que empiece a leer
            {
                char command = uart_getc(uart0); //obtiene el caracter obtenido por el bluetooth
            
                if(command == 'F') //condición que se mande una F para que el robot avance
                { 
                    motor_forward(MOT_DER_ADE, MOT_DER_ATR); //avanza el motor derecho
                    motor_forward(MOT_IZQ_ADE, MOT_IZQ_ATR); //avanza el motor izquierdo
                }

                else if(command == 'B') //condición que se mande una B para que el robot retroceda
                { 
                    motor_reverse(MOT_DER_ADE, MOT_DER_ATR); //retrocede el motor derecho
                    motor_reverse(MOT_IZQ_ADE, MOT_IZQ_ATR); //retrocede el motor izquierdo
                }

                else if(dato == 'R') //condición que se mande una R para que el robot gire a la derecha
                { 
                    motor_forward(MOT_IZQ_ADE, MOT_IZQ_ATR); //avanza el motor izquierdo
                    motor_reverse(MOT_DER_ADE, MOT_DER_ATR); //retrocede el motor derecho
                }

                else if(command == 'L') //condición que se mande una L para que el robot gire a la derecha
                { 
                    motor_forward(MOT_DER_ADE, MOT_DER_ATR); //avanza el motor derecho
                    motor_reverse(MOT_IZQ_ADE, MOT_IZQ_ATR); //retrocede el motor izquierdo
                }

                else if(command == 'S') //condición que se mande una S para que el pare
                {
                    motor_stop(MOT_IZQ_ADE, MOT_IZQ_ATR); //se frena el motor izquierdo
                    motor_stop(MOT_DER_ADE, MOT_DER_ATR); //se frena el motor derecho
                }

                else if(command == 'G') //condición que se mande una G para que el robot gire a la izquierda levemente
                {
                    motor_forward(MOT_DER_ADE, MOT_DER_ATR); //avanza el motor derecho
                    motor_stop(MOT_IZQ_ADE, MOT_IZQ_ATR); //se frena el motor izquierdo
                }

                else if(command == 'I') //condición que se mande una I para que el robot gire a la derecha levemente
                {
                    motor_forward(MOT_IZQ_ADE, MOT_IZQ_ATR); //avanza el motor izquierdo
                    motor_stop(MOT_DER_ADE, MOT_DER_ATR); //se frena el motor derecho
                }

                else if(command == 'H') //condición que se mande una H para que el robot retroceda hacia la izquierda levemente
                {
                    motor_reverse(MOT_DER_ADE, MOT_DER_ATR); //retrocede el motor derecho
                    motor_stop(MOT_IZQ_ADE, MOT_IZQ_ATR); //se frena el motor izquierdo
                }

                else if(command == 'J') //condición que se mande una J para que el robot retroceda hacia la derecha levemente
                {
                    motor_reverse(MOT_IZQ_ADE, MOT_IZQ_ATR); //retrocede el motor izquierdo
                    motor_stop(MOT_DER_ADE, MOT_DER_ATR); //se frena el motor derecho
                }

                else //condición si no se manda comando para que el robot se pare
                {
                    motor_stop(MOT_IZQ_ADE, MOT_IZQ_ATR); //se frena el motor izquierdo
                    motor_stop(MOT_DER_ADE, MOT_DER_ATR); //se frena el motor derecho
                }
            }

            else //si el bluetooth no lee, el robot se para
            {
                motor_stop(MOT_IZQ_ADE, MOT_IZQ_ATR); //se frena el motor izquierdo
                motor_stop(MOT_DER_ADE, MOT_DER_ATR); //se frena el motor derecho
            }
        }

        else //si el pin de modo está en 0 se ejecuta esta parte
        {
            if(distance < 75) //condición del ultrasónico para seguir una línea
            {
                i = 0; //el contador vuelve a 0 al ejecutarse esta parte

                if(infrarrojo_fren == 1 && infrarrojo_der == 1 && infrarrojo_izq == 1) //condición para que el robot avance 
                {
                    motor_forward(MOT_DER_ADE, MOT_DER_ATR); //avanza el motor derecho
                    motor_forward(MOT_IZQ_ADE, MOT_IZQ_ATR); //avanza el motor izquierdo
                }

                else if(infrarrojo_fren == 1 && infrarrojo_der == 0 && infrarrojo_izq == 0) // condición para que el robot avance
                {
                    motor_forward(MOT_DER_ADE, MOT_DER_ATR); //avanza el motor derecho
                    motor_forward(MOT_IZQ_ADE, MOT_IZQ_ATR); //avanza el motor izquierdo
                }   

                else if(infrarrojo_fren == 1 && infrarrojo_der == 1 && infrarrojo_izq == 0) //condición par que el robot gire levemente hacia la derecha
                {
                    motor_forward(MOT_IZQ_ADE, MOT_IZQ_ATR); //avanza el motor izquierdo
                    motor_stop(MOT_DER_ADE, MOT_DER_ATR); //se frena el motor derecho
                }

                else if(infrarrojo_fren == 1 && infrarrojo_der == 0 && infrarrojo_izq == 1) //condición para que el robot gire hacia la izquierda levemente
                {
                    motor_forward(MOT_DER_ADE, MOT_DER_ATR); //avanza el motor derecho
                    motor_stop(MOT_IZQ_ADE, MOT_IZQ_ATR); //se frena el motor izquierdo
                }

                else if(infrarrojo_fren == 0 && infrarrojo_der == 1  && infrarrojo_izq == 0) //condición para que el robot gire a la derecha
                {
                    motor_forward(MOT_IZQ_ADE, MOT_IZQ_ATR); //avanza el motor izquierdo
                    motor_reverse(MOT_DER_ADE, MOT_DER_ATR); //retrocede el motor derecho
                }   

                else if(infrarrojo_fren == 0 && infrarrojo_der == 0  && infrarrojo_izq == 1) //condición para que el robot gire a la izquierda
                {
                    motor_forward(MOT_DER_ADE, MOT_DER_ATR); //avanza el motor derecho
                    motor_reverse(MOT_IZQ_ADE, MOT_IZQ_ATR); //retrocede el motor izquierdo
                }   
            }

            else //si el ultrasónico no reconoce objto se ejecuta este apartado
            {
                absolute_time_t t0;

                if(i == 0) //detecta el instante en que pasa por primera vez
                {
                    t0 = get_absolute_time();
                }

                //el robot empieza a girar hacia la izquierda
		        motor_forward(MOT_DER_ADE, MOT_DER_ATR); //avanza el motor derecho
		        motor_reverse(MOT_IZQ_ADE, MOT_IZQ_ATR); //retrocede el motor izquierdo
                
                absolute_time_t t1 = get_absolute_time(); //instante en que pasa por este apartado

                if(absolute_time_diff_us(t0, t1)/1000 >= 3000) //condición por si se queda en este apartado por 3 s o más
                { 
                    //el robot avanza
                    motor_forward(MOT_DER_ADE, MOT_DER_ATR); //avanza el motor derecho
                    motor_forward(MOT_IZQ_ADE, MOT_IZQ_ATR); //avanza el motor izquierdo

                    sleep_ms(2000); //se esperan 2 s

                    i = 0; //el contador vuelve a 0
                }

                else //sino el contador sigue aumentando
                {
                i++;
                }
            }
        }
    }
    return 0; //fin del código
}

void ultrasonico_init(uint8_t trigger, uint8_t echo) //función para inicialiar ultrasonico
{
    gpio_init(trigger);  //se inicializa el pin de trigger
    gpio_set_dir(trigger, true); //se setea el pin de trigger como salida

    gpio_init(echo); //se inicializa el pin echo
    gpio_set_dir(echo, false); // se setea el pin de echo como entrada
}

float ultrasonico_get_distance_cm(uint8_t trigger, uint8_t echo) //función que usa el ultrasónico para la distancia
{
    gpio_put(trigger, true); //se pone el trigger en 1
    sleep_us(10); // se espera 10 us
    gpio_put(trigger, false); //se pone el trigger false

	absolute_time_t from, to;

    while(!gpio_get(echo))
    {
        from = get_absolute_time(); //instante cuando el echo no recibe el 1 del trigger
    }

    while(gpio_get(echo)) {
        to = get_absolute_time(); //instante en que recibe el 1 del trigger
    }

    int64_t pulse_width = absolute_time_diff_us(from, to); //calcula el ancho de pulso
    return pulse_width / 58.3; //devuelve la distancia en cm
}

void motor_init(uint8_t mot_ade, uint8_t mot_atr) //función para inicializar los motores
{
    gpio_init(mot_ade); //se inicializa el pin del motor que avanza
    gpio_set_dir(mot_ade, true); //se setea el pin del motor que avanza como salida

    gpio_init(mot_atr); //se inicializa el pin del motor que retrocede
    gpio_set_dir(mot_atr, true); //se inicializa el pin del motor que retrocede como salida
}

void motor_forward(uint8_t mot_ade, uint8_t mot_atr) //función que hace el motor avanzar
{
    gpio_put(mot_ade, true); //pone el motor para avanzar en 1
    gpio_put(mot_atr, false); //pone el motor para avanzar en 1
}

void motor_reverse(uint8_t mot_ade, uint8_t mot_atr) //función que hace el motor retroceder
{
    gpio_put(mot_ade, false); //pone el motor para avanzar en 0
    gpio_put(mot_atr, true); //pone el motor para retroceder en 1
}

void motor_stop(uint8_t mot_ade, uint8_t mot_atr) //función que hace parar el motor
{
    gpio_put(mot_ade, false); //pone el motor para avanzar en 0
    gpio_put(mot_atr, false); //pone el motor para retroceder en 0
}

void infrarrojo_init(uint8_t inf) //función que inicializa el innfrarrojo
{
    gpio_init(inf); //inicializa el pin del infrarrojo
    gpio_set_dir(inf, false); //setea el pin del infrarrojo como entrada
}

bool infrarrojo_get(uint8_t inf) //función que devuelve el valor del infrarrojo
{
    bool infrarrojo = gpio_get(inf); //obtiene el valor en el pin del infrarrojo
    return infrarrojo; //devuelve el valor booleano del infrarrojo
}

void bluetooth_init(uint8_t tx, uint8_t rx, uint8_t mode) //función que inicializa el modulo bluetooth
{
    uart_init(uart0, 9600); //inicializa el uart0 a 9600 baudios

    gpio_set_function(tx, GPIO_FUNC_UART); //establece el pin como tx del uart0
    gpio_set_function(rx, GPIO_FUNC_UART); //establece el pin como rx del uart0

    gpio_init(mode); //inicializa el pin de modo
    gpio_set_dir(mode, false); //setea el pin de modo como entrada

    uart_set_fifo_enabled(uart0, true); //hablilita la conexxión fifo
}

bool bluetooth_mode_get(uint8_t mode) //función que obtiene el valor de pin de modo
{
    bool bluetooth_mode = gpio_get(mode); //obtiene el valor del pin de modo
    return bluetooth_mode;
}
