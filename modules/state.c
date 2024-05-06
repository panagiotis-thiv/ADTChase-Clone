
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


#include "ADTVector.h"
#include "ADTList.h"
#include "state.h"
#include "vec2.h"


// Οι ολοκληρωμένες πληροφορίες της κατάστασης του παιχνιδιού.
// Ο τύπος State είναι pointer σε αυτό το struct, αλλά το ίδιο το struct
// δεν είναι ορατό στον χρήστη.

struct state {
	Vector objects;			// περιέχει στοιχεία Object (αστεροειδείς, σφαίρες)
	struct state_info info;	// Γενικές πληροφορίες για την κατάσταση του παιχνιδιού
	int next_bullet;		// Αριθμός frames μέχρι να επιτραπεί ξανά σφαίρα
	float speed_factor;		// Πολλαπλασιαστής ταχύτητς (1 = κανονική ταχύτητα, 2 = διπλάσια, κλπ)
};


// Δημιουργεί και επιστρέφει ένα αντικείμενο

static Object create_object(ObjectType type, Vector2 position, Vector2 speed, Vector2 orientation, double size) {
	Object obj = malloc(sizeof(*obj));
	obj->type = type;
	obj->position = position;
	obj->speed = speed;
	obj->orientation = orientation;
	obj->size = size;
	return obj;
}

// Επιστρέφει έναν τυχαίο πραγματικό αριθμό στο διάστημα [min,max]

static double randf(double min, double max) {
	return min + (double)rand() / RAND_MAX * (max - min);
}

// Προσθέτει num αστεροειδείς στην πίστα (η οποία μπορεί να περιέχει ήδη αντικείμενα).
//
// ΠΡΟΣΟΧΗ: όλα τα αντικείμενα έχουν συντεταγμένες x,y σε ένα καρτεσιανό επίπεδο.
// - Η αρχή των αξόνων είναι η θέση του διαστημόπλοιου στην αρχή του παιχνιδιού
// - Στο άξονα x οι συντεταγμένες μεγαλώνουν προς τα δεξιά.
// - Στον άξονα y οι συντεταγμένες μεγαλώνουν προς τα πάνω.

static void add_asteroids(State state, int num) {
	for (int i = 0; i < num; i++) {
		// Τυχαία θέση σε απόσταση [ASTEROID_MIN_DIST, ASTEROID_MAX_DIST]
		// από το διστημόπλοιο.
		//
		Vector2 position = vec2_add(
			state->info.spaceship->position,
			vec2_from_polar(
				randf(ASTEROID_MIN_DIST, ASTEROID_MAX_DIST),	// απόσταση
				randf(0, 2*PI)									// κατεύθυνση
			)
		);

		// Τυχαία ταχύτητα στο διάστημα [ASTEROID_MIN_SPEED, ASTEROID_MAX_SPEED]
		// με τυχαία κατεύθυνση.
		//
		Vector2 speed = vec2_from_polar(
			randf(ASTEROID_MIN_SPEED, ASTEROID_MAX_SPEED) * state->speed_factor,
			randf(0, 2*PI)
		);

		Object asteroid = create_object(
			ASTEROID,
			position,
			speed,
			(Vector2){0, 0},								// δεν χρησιμοποιείται για αστεροειδείς
			randf(ASTEROID_MIN_SIZE, ASTEROID_MAX_SIZE)		// τυχαίο μέγεθος
		);
		vector_insert_last(state->objects, asteroid);
	}
}

// Δημιουργεί και επιστρέφει την αρχική κατάσταση του παιχνιδιού

State state_create() {
	// Δημιουργία του state
	State state = malloc(sizeof(*state));
	// Γενικές πληροφορίες
	state->info.paused = false;				// Το παιχνίδι ξεκινάει χωρίς να είναι paused.
	state->speed_factor = 1;				// Κανονική ταχύτητα
	state->next_bullet = 0;					// Σφαίρα επιτρέπεται αμέσως
	state->info.score = 0;				// Αρχικό σκορ 0

	// Δημιουργούμε το vector των αντικειμένων, και προσθέτουμε αντικείμενα
	state->objects = vector_create(0, NULL);

	// Δημιουργούμε το διαστημόπλοιο
	state->info.spaceship = create_object(
		SPACESHIP,
		(Vector2){0, 0},			// αρχική θέση στην αρχή των αξόνων
		(Vector2){0, 0},			// μηδενική αρχική ταχύτητα
		(Vector2){0, 1},			// κοιτάει προς τα πάνω
		SPACESHIP_SIZE				// μέγεθος
	);

	// Προσθήκη αρχικών αστεροειδών
	add_asteroids(state, ASTEROID_NUM);

	return state;
}

// Επιστρέφει τις βασικές πληροφορίες του παιχνιδιού στην κατάσταση state

StateInfo state_info(State state) {
	return &state->info;
}

// Επιστρέφει μια λίστα με όλα τα αντικείμενα του παιχνιδιού στην κατάσταση state,
// των οποίων η θέση position βρίσκεται εντός του παραλληλογράμμου με πάνω αριστερή
// γωνία top_left και κάτω δεξιά bottom_right.

List state_objects(State state, Vector2 top_left, Vector2 bottom_right) {

	List result = list_create(NULL);
	// Vector2 top_right = {bottom_right.x, top_left.y};
	// Vector2 bottom_left = {top_left.x, bottom_right.y};
	// Rectangle box_area = {top_left.x, top_left.y, vec2_distance(top_left, top_right), vec2_distance(bottom_left, bottom_right)};

	for (int i = 0; i < vector_size(state->objects); i++) {
		Object obj = vector_get_at(state->objects, i);
		if (obj != NULL) {
			if(
				obj->position.x >= top_left.x && obj->position.x <= bottom_right.x &&
				obj->position.y <= top_left.y && obj->position.y >= bottom_right.y	
			) {
				list_insert_next(result, LIST_BOF, obj);
			}
		}
			
	}
	
	return result;
}

//Αντιστρέφει δύο vectors
void vector_swap(Vector vec, int pos1, int pos2) {

	if (pos1 == pos2)
		return;

	Pointer obj1 = vector_get_at(vec, pos1);
	Pointer obj2 = vector_get_at(vec, pos2);

	vector_set_at(vec, pos1, obj2);
	vector_set_at(vec, pos2, obj1);
}


// Ενημερώνει την κατάσταση state του παιχνιδιού μετά την πάροδο 1 frame.
// Το keys περιέχει τα πλήκτρα τα οποία ήταν πατημένα κατά το frame αυτό.

void state_update(State state, KeyState keys) {
	
	//Ελέγχος άμα είναι πατημένο το p ώστε το παιχνίδι να σταματήσει ή όχι.
	if (keys->p) 
		state->info.paused = !state->info.paused;

	if (state->info.paused == false || keys->n) {

		//Αρχικά ανανεώνω την θέση κάθε αντικειμένου. Μετά ελέγχω άμα πρέπει να δημιουργηθούν αστεροειδείς. 
		//Μετράω πόσοι υπάρχουν από την θέση του διαστημόπλοιου σε ακτίνα ASTEROID_MAX_DIST και άμα είναι 
		//μικρότεροι του 6 δημιουργώ όσοι χρειάζονται.
		//Στο τέλος ανανεώνω και την θέση για το διαστημόπλοιο.

		Vector2 top_left = vec2_add(state->info.spaceship->position, (Vector2){-ASTEROID_MAX_DIST,ASTEROID_MAX_DIST});
		Vector2 bottom_right = vec2_add(state->info.spaceship->position, (Vector2){ASTEROID_MAX_DIST,-ASTEROID_MAX_DIST});
		int countAsteroid = 0;

		for (int i = 0; i < vector_size(state->objects); i++) {
			Object obj = vector_get_at(state->objects, i);
			if (obj != NULL) {
				obj->position = vec2_scale(vec2_add(obj->position, obj->speed), state->speed_factor);
				if(
					obj->position.x >= top_left.x && obj->position.x <= bottom_right.x &&
					obj->position.y <= top_left.y && obj->position.y >= bottom_right.y &&
					obj->type == ASTEROID
				)
				countAsteroid++;
			}
		}
		if (countAsteroid < ASTEROID_NUM) {
			add_asteroids(state, ASTEROID_NUM-countAsteroid);
			state->info.score += ASTEROID_NUM-countAsteroid;
		}
	
		state->info.spaceship->position = vec2_scale(vec2_add(state->info.spaceship->position, state->info.spaceship->speed),state->speed_factor);


		//Αύξηση ταχύτητας παιχνιδιού άμα το σκορ είναι πολλαπλάσιο του 100
		if (state->info.score != 0) {
			if (state->info.score % 100 == 0) 
				state->speed_factor += state->speed_factor * 0.1;
		}
		
		//Έλεγχος για όταν κάποιο κουμπί είναι πατημένο.

		if (keys->right)
			state->info.spaceship->orientation = vec2_rotate(state->info.spaceship->orientation, SPACESHIP_ROTATION);
		if (keys->left) 
			state->info.spaceship->orientation = vec2_rotate(state->info.spaceship->orientation, -SPACESHIP_ROTATION);
		if (keys->up) {
			Vector2 accel = vec2_scale(state->info.spaceship->orientation, SPACESHIP_ACCELERATION);
			state->info.spaceship->speed = vec2_add(state->info.spaceship->speed, accel);
		}
		else 
			state->info.spaceship->speed = vec2_scale(state->info.spaceship->speed, SPACESHIP_SLOWDOWN);
		if (keys->space) {
			if (state->next_bullet == 0) {
				Vector2 speed = vec2_add(state->info.spaceship->speed, vec2_scale(state->info.spaceship->orientation, BULLET_SPEED));
				Vector2 position = state->info.spaceship->position;

				Object bullet = create_object(
					BULLET,
					position,
					speed,
					(Vector2){0, 0},								
					BULLET_SIZE
				);
				vector_insert_last(state->objects, bullet);

				state->next_bullet++;
			}
			else if (state->next_bullet == BULLET_DELAY)
				state->next_bullet = 0;
			else
				state->next_bullet++;
		}

		// //Έλεγχος συγκρούσεων
		
		bool collisionAsteroidSpaceship; 
		bool collisionAsteroidBullet;
		
		for (int i = 0; i < vector_size(state->objects); i++) {
			Object obj = vector_get_at(state->objects, i);
			collisionAsteroidSpaceship = false;
			//Αστεροειδής - Διαστημόπλοιο
			if (obj != NULL) {
				if(obj->type == ASTEROID)
					collisionAsteroidSpaceship = CheckCollisionCircles(obj->position, (obj->size)/2, state->info.spaceship->position, SPACESHIP_SIZE/2);
				if (collisionAsteroidSpaceship) {
					vector_set_at(state->objects, i, NULL);
					if (state->info.score < 0) 
						state->info.score = (state->info.score/2) * (-1);
					else 
						state->info.score = (state->info.score/2);
				}	
			}
		

			//Αστεροειδής - Σφαίρα
			for (int j = 0; j < vector_size(state->objects); j++) {
				Object obj2 = vector_get_at(state->objects, j);
				collisionAsteroidBullet = false;
				if (obj != NULL && obj2 != NULL) {
					if (obj->type == ASTEROID && obj2->type == BULLET ) 
						collisionAsteroidBullet = CheckCollisionCircles(obj->position, (obj->size)/2, obj2->position, BULLET_SIZE/2);					
					if (collisionAsteroidBullet) {
						//Δημιουργώ τους καινούργιους αστεροειδείς
						for (int k = 0; k < 2; k++) {

							// Τυχαία κατεύθυνση και μήκος 1,5 φορά μεγαλύτερο της ταχύτητας του αρχικού.
							Vector2 speed = vec2_from_polar(
								obj->speed.x * 1.5,
								randf(0, 2*PI)
							);

							Object asteroid = create_object(
								ASTEROID,
								obj->position,									//Η θέση τους θα είναι η θέση του αστεροειδή που συγκρούστηκε
								speed,
								(Vector2){0, 0},								//Δεν χρησιμοποιείται για αστεροειδείς
								randf(ASTEROID_MIN_SIZE, obj->size/2)		    //Τυχαίο μέγεθος μέχρι το μέγεθος του αστεροειδή που συγκρούστηκε δία 2
							);
							vector_insert_last(state->objects, asteroid);
						}

						if ((state->info.score-10) < 0)
							state->info.score = 0;
						else
							state->info.score = state->info.score-10;

						//Καταστρέφω αστεροειδή και σφαίρα
						vector_set_at(state->objects, i, NULL);
						vector_set_at(state->objects, j, NULL);
					}
				}
			}
		}

		//Διαγραφή NULL κόμβων
		for (int i = 0; i < vector_size(state->objects); i++) {
			Object obj = vector_get_at(state->objects, i);
			Object last_obj = vector_get_at(state->objects, vector_size(state->objects)-1);
			while (last_obj == NULL) {
				vector_remove_last(state->objects);
				last_obj = vector_get_at(state->objects, vector_size(state->objects)-1);
			}
			if (obj == NULL) {
				vector_swap(state->objects, i, vector_size(state->objects)-1);
				vector_remove_last(state->objects);
				i--;
			}
		}
	}
}

// Καταστρέφει την κατάσταση state ελευθερώνοντας τη δεσμευμένη μνήμη.

void state_destroy(State state) {
	// Προς υλοποίηση
}
