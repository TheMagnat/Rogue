
std::vector<sf::IntRect> creerVect(int start, int ligne, int longueur, int hauteur, int ecart, int nbSprite){
	int i;

	std::vector<sf::IntRect> v;
	for(i=0; i<nbSprite; ++i){
		v.push_back(sf::IntRect(start+i*ecart, ligne, longueur, hauteur));
	}

	return v;
}

class Animation{

	public:
		Animation(std::vector<std::vector<sf::IntRect>> sens) :
		actuelle_(0), sens_(sens) {
			for(size_t i = 0; i<sens.size(); ++i){
				actuSens_.push_back(0);
			}
		}

		void setActuelle(int const& quelleSens){
			actuSens_[actuelle_] = 0;
			actuelle_ = quelleSens;
		}

		sf::IntRect& actuelSprite(){
			return sens_[actuelle_][actuSens_[actuelle_]];
		}

		sf::IntRect& nextSprite(){
			size_t& save = actuSens_[actuelle_];

			if(++save == sens_[actuelle_].size()) save = 0;

			return sens_[actuelle_][save];
		}

		int getActuelle(){
			return actuelle_;
		}


	private:
		//Rien, Bas, Haut, Cote, action

		int actuelle_; //Position en cours

		std::vector<size_t> actuSens_; //Numero du sprite

		std::vector<std::vector<sf::IntRect>> sens_; //array contenant les vecteur de rectangle
};