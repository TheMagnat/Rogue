#include "head.h"

static Vector2f POS_HERO(432, 332);

static std::vector<std::vector<IntRect>> rogueRect {{creerVect(8, 0, 16, 32, 32, 10), creerVect(8, 160, 16, 32, 32, 8), creerVect(8, 192, 16, 32, 32, 8), creerVect(8, 224, 16, 32, 32, 8), creerVect(8, 96, 22, 32, 32, 10)}};


int main(int argc, char const *argv[]){

	int save;

	std::cout << "CLEF DE GENERATION ALEATOIRE : " << time(NULL) << std::endl;
	srand(time(NULL));

	RenderWindow app(VIDEO_MODE, "Rogue by Magnat");
	app.setVerticalSyncEnabled(true);

	save = menu(app);
	if(save == 1){
		//Preparartion pour le SoundHandler
		SoundHandler::initialiserSons(FICHIER_SON);


		//Preparation pour le personnage
		Arme dague(10);
		Animation animRogue(rogueRect);

		//Creation du personnage
		Personnage rogue("ressources/rogue.png", animRogue, POS_HERO, 100);
		rogue.getArme(&dague); //On lui equipe arme de base

		donjon(app, rogue);
	}
	app.close();
	return 0;
}