#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <array>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <string>
#include <list>
#include <memory>
#include <unordered_map>

//#include <thread>

#include <iostream>

#include "TrieCollision.hpp"

#include "Hud.hpp"
#include "fondu.hpp"
#include "Animation.hpp"
#include "SoundHandler.hpp"

#include "particule.hpp"
#include "Projectile.hpp"
#include "Arme.hpp"
#include "Personnage.hpp"
#include "Ennemie.hpp"
#include "Objet.hpp"
#include "Case.hpp"
#include "Salle.hpp"
#include "CollisionManager.hpp"
#include "IA.hpp"
#include "Monde.hpp"

using namespace sf;

#define SPEED 75

static Vector2f CAM_TAILLE(200, 150); // 200, 150

static Vector2f TAILLE_ECRAN(800, 600);
static VideoMode VIDEO_MODE(800, 600, 32); // 800, 600

static std::vector<std::string> FICHIER_SON {"ressources/coin/handleCoins.ogg", "ressources/coin/handleCoins2.ogg", 
"ressources/smoke/steam hisses - Marker #1.wav", "ressources/smoke/steam hisses - Marker #2.wav", "ressources/smoke/steam hisses - Marker #3.wav", "ressources/smoke/steam hisses - Marker #4.wav", "ressources/smoke/steam hisses - Marker #5.wav",
"ressources/swing/swing1.wav", "ressources/swing/swing2.wav", "ressources/swing/swing3.wav",
"ressources/death/1.ogg", "ressources/death/2.ogg", "ressources/death/3.ogg", "ressources/death/4.ogg", "ressources/death/5.ogg", "ressources/death/6.ogg", "ressources/death/7.ogg", "ressources/death/8.ogg", "ressources/death/9.ogg",
"ressources/hurt/Socapex - hurt.wav"};

#include "waiting.cpp"
#include "menu.cpp"
#include "donjon.cpp"
