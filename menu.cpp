

int menu(RenderWindow& app) {
	size_t indexChoix(0);
	size_t i, taille;
	bool modeChoix(true);
	Text* choix;
	sf::Vector2f tempo;
	Clock temps;

	Texture fondecran_;
	Sprite fondecran;
	Sprite fondecran2;

	if(!fondecran_.loadFromFile("ressources/menupaper.png")){

	}
	else{
		fondecran.setTexture(fondecran_);
		fondecran.setPosition(0, -400);

		fondecran2.setTexture(fondecran_);
		fondecran2.setPosition(1920, -400);
	}

	Font font;
	std::vector<Text*> texts;

	if(!font.loadFromFile("ressources/FFFFORWA.TTF")){
		return -1;
	}
	else{
		texts.push_back(new Text("Lancer une partie !", font));
		texts.back()->setOrigin({ texts.back()->getLocalBounds().width/2, texts.back()->getLocalBounds().height/2 });
		texts.back()->setPosition(400, 200);
		choix = texts.back();
		choix->setFillColor(Color::White);

		texts.push_back(new Text("Test !", font));
		texts.back()->setOrigin({ texts.back()->getLocalBounds().width/2, texts.back()->getLocalBounds().height/2 });
		texts.back()->setPosition(400, 300);
		texts.back()->setFillColor(Color::Black);

		texts.push_back(new Text("By Magnat", font));
		texts.back()->setOrigin({ texts.back()->getLocalBounds().width/2, texts.back()->getLocalBounds().height/2 });
		texts.back()->setPosition(400, 400);
		texts.back()->setFillColor(Color::Black);
	}
	taille = texts.size();

	Event event;

	temps.restart();
	while(app.isOpen()){

		while(app.pollEvent(event)){
			if(event.type == Event::Closed){
				app.close();
			}
			if(event.type == Event::KeyPressed){
				switch(event.key.code){

					case Keyboard::Up :
						if(indexChoix==0){
							indexChoix=taille-1;
						}
						else{
							--indexChoix;
						}
						break;

					case Keyboard::Down :
						if(indexChoix >= taille) indexChoix = 0;
						else ++indexChoix;
						break;

					case Keyboard::Enter :
						if(indexChoix == 0){
							return 1;
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
		
		if(texts[indexChoix] != choix){
			temps.restart();
			modeChoix=true;
			choix->setScale(1, 1);
			choix->setFillColor(Color::Black);
			choix = texts[indexChoix];
			choix->setFillColor(Color::White);

		}

		if(temps.getElapsedTime().asSeconds()>0.01){
			temps.restart();
			tempo = choix->getScale();
			if(modeChoix){
				if(tempo.x >= 1.2){
					modeChoix=false;
					choix->setScale(tempo.x-0.01, tempo.y-0.01);
				}
				else{
					choix->setScale(tempo.x+0.01, tempo.y+0.01);
				}
			}
			else{
				if(tempo.x <= 0.7){
					modeChoix=true;
					choix->setScale(tempo.x+0.01, tempo.y+0.01);
				}
				else{
					choix->setScale(tempo.x-0.01, tempo.y-0.01);
				}
			}
		}


		if(fondecran.getPosition().x <= -1920){
			fondecran.setPosition(fondecran2.getPosition().x+1920, -400);
		}
		if(fondecran2.getPosition().x == -1920){
			fondecran2.setPosition(fondecran.getPosition().x+1920, -400);
		}
		fondecran.move(-2, 0);
		fondecran2.move(-2, 0);

		app.draw(fondecran);
		app.draw(fondecran2);

		for(i=0; i<taille; ++i){
			app.draw(*texts[i]);
		}

		app.display();
	}
	return 0;
}