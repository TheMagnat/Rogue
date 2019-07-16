
/*void donjonDrawBack(RenderWindow& app, Monde* const& mondeActuelle, Personnage const& rogue, View& vue, bool& go){
	while(app.isOpen()){
		while(!go);
		std::cout << "drawback\n";
		//app.setActive(true);
		vue.setCenter(rogue.getPosition());
		app.setView(vue);
		app.clear();
		mondeActuelle->drawArrierePlan(app);
		app.setActive(false);
		go=false;
	}
}

void donjonActualise(RenderWindow const& app, Monde* const& mondeActuelle, float const& tempsPasser, bool& go){
	while(app.isOpen()){
		while(!go);
		mondeActuelle->actualise(tempsPasser);
		go=false;
	}
}*/

int donjon(RenderWindow& app, Personnage& rogue /*std::vector<Monde*>& univers*/){

	Fondu fondu(1);

	int tempo;
	float tempsPasser, x, y;
	bool sneak; //goB(false), goI(false);

	//Musique et Ambience
	Music music, ambience;
	if(!music.openFromFile("ressources/mystic.ogg"))
	    std::cerr << "Impossible d'ouvrir ressources/mystic.wav" << std::endl;
	else{
		music.setLoop(true);
		music.play();
	}

	if(!ambience.openFromFile("ressources/dungeon2.ogg"))
	    std::cerr << "Impossible d'ouvrir ressources/dungeon.ogg" << std::endl;
	else{
		ambience.setLoop(true);
		ambience.play();
	}

	//Pointeurs utiles
	Clock clock, move_clock; //Clock d'annimation et de gestion de temps par frame

	Event event;

	//Pointeur du monde actuelle et vecteur de pointeur de monde
	Monde* mondeActuelle;
	std::vector<std::shared_ptr<Monde>> univers;

	//Creation du monde
	univers.emplace_back(new Monde("ressources/dungeon.png", &rogue));
	mondeActuelle = univers.back().get();
	mondeActuelle->remplirAleatoire(50);
	rogue.setPosition(mondeActuelle->getPosEscaHaut().x+8, mondeActuelle->getPosEscaHaut().y+32);


	//Creation du gestionaire de collision du personnage
	//CollisionManager colli(&rogue, mondeActuelle);
	CollisionManager::setJoueur(&rogue);
	CollisionManager::setMondeActuel(&mondeActuelle->getObjets(), &mondeActuelle->getSalles(), &mondeActuelle->getChemins());

	//Creation de la vue
	View vue(rogue.getPosition(), CAM_TAILLE);
	app.setView(vue);

	//On redemarre les Clock et on lance le programme
	move_clock.restart();
	clock.restart();

	fondu.setActif(true);

	
	std::list<Particule*> particules;

	//TEST
	Hud hud_("ressources/hud.png", TAILLE_ECRAN);



	CollisionManager::setColli(&mondeActuelle->getColli());

	//FIN TEST

	//std::thread tBack(donjonDrawBack, std::ref(app), std::ref(mondeActuelle), std::ref(rogue), std::ref(vue), std::ref(goB));
	//std::thread tIA(donjonActualise, std::ref(app), std::ref(mondeActuelle), std::ref(tempsPasser), std::ref(goI));

	while(app.isOpen()){

		while(app.pollEvent(event)){
			if(event.type == Event::Closed){
				app.close();
			}
			if(event.type == Event::KeyPressed){
				switch(event.key.code){

					case Keyboard::P :
						std::cout << rogue.getPosition().x << " ET " << rogue.getPosition().y << std::endl;
						std::cout << rogue.getHitBox().left << " ET " << rogue.getHitBox().top << std::endl;
						break;

					case Keyboard::Tab :
						if(hud_.isActif()){
							hud_.ferme();
						}
						else{
							hud_.ouvre();
						}
						break;

					case Keyboard::Escape :
						app.close();
						break;

					default:
						break;
				}
			}
		}

		tempsPasser = move_clock.restart().asSeconds();

		//Gestion de l'attaque du personnage
		x=0;
		if(Keyboard::isKeyPressed(Keyboard::J)){
			x=-1;
		}
		if(Keyboard::isKeyPressed(Keyboard::L)){
			x=1;
		}
		if(x != 0){
			rogue.attack(x);
		}

		//Gestion des déplacements du personnage
		x=0; y=0;
		if(Keyboard::isKeyPressed(Keyboard::Space)){
			rogue.lanceSmoke(particules);
		}
		if(Keyboard::isKeyPressed(Keyboard::Z)){
			y=-SPEED;
		}
		if(Keyboard::isKeyPressed(Keyboard::S)){
			y=SPEED;
		}
		if(Keyboard::isKeyPressed(Keyboard::Q)){
			x=-SPEED;
		}
		if(Keyboard::isKeyPressed(Keyboard::D)){
			x=SPEED;
		}
		if(Keyboard::isKeyPressed(Keyboard::LShift)){
			sneak=true;
			x = ((int)(x/2*tempsPasser*100))/100.f;
			y = ((int)(y/2*tempsPasser*100))/100.f;
		}
		else{
			sneak=false;
			x = ((int)(x*tempsPasser*100))/100.f;
			y = ((int)(y*tempsPasser*100))/100.f;
		}

		if(rogue.move(x, y, sneak)){

		}

		tempo=CollisionManager::verifCollision(x, y);
		if(tempo==1){
			rogue.annulMov(x, y);
		}
		else if(tempo==10){ //Descendre
			if(mondeActuelle->getPtrBas()){//Si la salle a déjà été généré

				mondeActuelle=mondeActuelle->getPtrBas().get();
				rogue.setPosition(mondeActuelle->getPosEscaHaut().x+8, mondeActuelle->getPosEscaHaut().y+32);
			
			}
			else{ //Creation d'une nouvelle salle
				univers.emplace_back(new Monde("ressources/dungeon.png", &rogue, mondeActuelle));
				mondeActuelle->setPtrBas(univers.back());
				mondeActuelle = univers.back().get();

				mondeActuelle->remplirAleatoire(20);

				rogue.setPosition(mondeActuelle->getPosEscaHaut().x+8, mondeActuelle->getPosEscaHaut().y+32);
			}

			CollisionManager::setMondeActuel(&mondeActuelle->getObjets(), &mondeActuelle->getSalles(), &mondeActuelle->getChemins());

			waiting(app, "Etage inferieur", 1);
			fondu.reset();
			fondu.setActif(true);
			rogue.restart();
			move_clock.restart();
		}
		else if(tempo==20){ //Monter
			if(mondeActuelle->getPtrHaut()){
				mondeActuelle=mondeActuelle->getPtrHaut().get();
				CollisionManager::setMondeActuel(&mondeActuelle->getObjets(), &mondeActuelle->getSalles(), &mondeActuelle->getChemins());
				rogue.setPosition(mondeActuelle->getPosEscaBas().x+8, mondeActuelle->getPosEscaBas().y+32);
			}
			else{
				return 0;
			}
			waiting(app, "Etage superieur", 1);
			fondu.reset();
			fondu.setActif(true);
			rogue.restart();
			move_clock.restart();
		}
		
		if(rogue.getArme()->isActif()){
			mondeActuelle->attack(rogue.getArme()->getHitBox());
		}

		mondeActuelle->actualise(tempsPasser);

		//while(goB);
		//app.setActive(true);
		
		vue.setCenter(rogue.getPosition());

		
		app.setView(vue);
		app.clear();

		mondeActuelle->draw(app, rogue);

		for(std::list<Particule*>::iterator it = particules.begin(); it != particules.end(); it++){
			if((*it)->actualise()){
				particules.erase(it);
				--it;
			}
			else{
				app.draw((*it)->getSprite());
			}
		}

		if(fondu.isActif()){
			fondu.setPosition(vue.getCenter().x, vue.getCenter().y);

			app.draw(fondu.getRectangle());
		}

		if(hud_.isActif()){

			app.setView(app.getDefaultView());
			hud_.draw(app);
		}

		app.display();

	}
	return 0;
}