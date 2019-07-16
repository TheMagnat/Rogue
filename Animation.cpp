#include "Animation.hpp"

Animation::Animation(std::vector<std::vector<sf::IntRect>> sens) :
actuelle_(0), sens_(sens) {
	for(size_t i = 0; i<sens.size(); ++i){
		actuSens_.push_back(0);
	}
}

void Animation::setActuelle(int const& quelleSens){
	actuSens_[actuelle_] = 0;
	actuelle_ = quelleSens;
}

sf::IntRect& Animation::actuelSprite(){
	return sens_[actuelle_][actuSens_[actuelle_]];
}

sf::IntRect& Animation::nextSprite(){
	size_t& save = actuSens_[actuelle_];

	if(++save == sens_[actuelle_].size()) save = 0;

	return sens_[actuelle_][save];
}

int Animation::getActuelle(){
	return actuelle_;
}

std::vector<sf::IntRect> creerVect(int start, int ligne, int longueur, int hauteur, int ecart, int nbSprite){
	int i;

	std::vector<sf::IntRect> v;
	for(i=0; i<nbSprite; ++i){
		v.push_back(sf::IntRect(start+i*ecart, ligne, longueur, hauteur));
	}

	return v;
}