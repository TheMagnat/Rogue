

int waiting(RenderWindow& app, std::string const& aAfficher, float const& cbTemps){

	app.setView(app.getDefaultView());

	Font font;
	Text texte;

	texte.setString(aAfficher);

	if(!font.loadFromFile("ressources/FFFFORWA.TTF")){
		std::cerr << "Impossible de charger ressources/FFFFORWA.TTF";
		//Text texte(aAfficher);
	}
	else{
		texte.setFont(font);
	}
	texte.setOrigin({ texte.getLocalBounds().width/2, texte.getLocalBounds().height/2 });
	texte.setPosition(app.getDefaultView().getCenter());

	Clock temps;

	while(app.isOpen()){

		if(temps.getElapsedTime().asSeconds() > cbTemps){
			return 1;
		}

		app.clear();

		app.draw(texte);

		app.display();
	}
	return 0;
}