#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>

#include <wiringPi.h>
#include <sched.h>

//initialisation du pin de reception
int pin;

//Fonction de passage du programme en temps réel (car la reception se joue a la micro seconde près)
void scheduler_realtime() {
    struct sched_param p;
    p.__sched_priority = sched_get_priority_max(SCHED_RR);
    if ( sched_setscheduler( 0, SCHED_RR, &p ) == -1 ) {
	perror("Failed to switch to realtime scheduler.");
    }
}

//Fonction de remise du programme en temps standard
void scheduler_standard() {
    struct sched_param p;
    p.__sched_priority = 0;
    if ( sched_setscheduler( 0, SCHED_OTHER, &p ) == -1 ) {
	perror("Failed to switch to normal scheduler.");
    }
}

// Recuperation du temps (en micro secondes) d'une pulsation
int pulseIn(int pin, int level, int timeout)
{
    struct timeval tn, t0, t1;
    long micros;
    gettimeofday(&t0, NULL);
    micros = 0;

    // On boucle pour detecter le passage de 1 a 0
    while (digitalRead(pin) != level)
    {
	gettimeofday(&tn, NULL);
	if (tn.tv_sec > t0.tv_sec) micros = 1000000L; else micros = 0;
	micros += (tn.tv_usec - t0.tv_usec);
	if (micros > timeout) return 0;
    }
    gettimeofday(&t1, NULL);

    while (digitalRead(pin) == level)
    {
	gettimeofday(&tn, NULL);
	if (tn.tv_sec > t0.tv_sec) micros = 1000000L; else micros = 0;
	micros = micros + (tn.tv_usec - t0.tv_usec);
	if (micros > timeout) return 0;
    }
    if (tn.tv_sec > t1.tv_sec) micros = 1000000L; else micros = 0;
    micros = micros + (tn.tv_usec - t1.tv_usec);
    return micros;
}

// TODO 1: Recuperer les 64 bits avant de cree le packet
// TODO 2: Faire un objet packet 
// TODO 3: Opti
int main (int argc, char** argv)
{
    // On passe en temps reel
    scheduler_realtime();

    // Recuperation du pin gpio
    pin = (argc == 2) ? atoi(argv[1]) : 1;

    // Chargement de la lib
    if (wiringPiSetup() == -1)
    {
        fprintf(stderr, "Librairie Wiring PI introuvable, veuillez lier cette librairie...");
        return -1;
    }

    // Configuration de la pin en input
    pinMode(pin, INPUT);

    // Boucle de reception
    for (;;)
    {
	int i;

	// Variable stoquant la duree de la derniere reception
	unsigned long t = 0;

	//avant dernier byte recu
	int prevBit = 0;

	//dernier byte recu
	int bit = 0;

	// Donnee d'un packet 
	unsigned long id = 0;
	unsigned long value = 0;
	unsigned long module = 0;

	// Recuperation du temps que que le signal reste dans la possition param2 (LOW ou HIGH)
	t = pulseIn(pin, LOW, 100000);

	// Attente d'un verrou
	while((t < 2700 || t > 2800))
	    t = pulseIn(pin, LOW, 100000);

	// Boucle de recuperation du packet
	for(i = 0; i < 64; ++i)
	{
	    t = pulseIn(pin, LOW, 100000);

	    //Definition du bit (0 ou 1)
	    if (t > 200 && t < 350) // Valeur theorique attendu 275
		bit = 1;
	    else if (t > 1100 && t < 1350) // Valeur theorique attendu 1225
		bit = 0;
	    else
	    {
		// Debug if fail
		/*
		   std::cout << "fail, i = " << i << std::endl;
		   std::cout << "bad value " << t << " timeout?" << std::endl;
		*/
		i = 0;
		break;
	    }

	    // 0 = 01, 1 = 10, on utilisera donc prevBit pour avoir notre valeur
	    if (i % 2 == 1)
	    {
		if ((prevBit ^ bit) == 0)
		{
		    // Debug if fail
		    //std::cout << "fail manchester, i = " << i << std::endl;
		    i = 0;
		    break;
		}
		if (i < (16 * 2) + 1)
		{
		    // les 16 premiers bits correspondent a l'id
		    id <<= 1; // Decale ce qu'on a deja
		    id |= prevBit; // Assigne le bit d'apres
		}      
		else if (i < (16 * 3) + 1)
		{
		    // les 8 bits suivant correspondent au module
		    module <<= 1; // Decale ce qu'on a deja
		    module |= prevBit; // Assigne le bit d'apres
		}      
		else
		{
		    // les 8 derniers bit correspondent a la data
		    value <<= 1; // Decale ce qu'on a deja
		    value|= prevBit; // Assigne le bit d'apres
		}
	    }

	    prevBit = bit;
	}
	// Si le packet est recu, on affiche son contenu
	if (i == 64)
	    printf("Packet recu -> [id = %ld, module = %ld, value = %ld]\n", id, module, value);
    }
    scheduler_standard();
}

