#include "model/Pitch.h"
#include "util/Constants.h"


Pitch::Pitch(Camera* camera)
{
	this->localTeam = NULL;
	this->awayTeam = NULL;
	this->camera = camera;
}


void Pitch::setTeam(Team* team, int teamNumber) {
    if (teamNumber == 0) {
        this->localTeam = team;
    } else if (teamNumber == 1) {
        this->awayTeam = team;
    }
}

Team* Pitch::getTeam(int teamNumber) {
  if (teamNumber == 0) {
      return this->localTeam;
    }
  else {
      return this->awayTeam;
    }
}

void Pitch::setBall(Ball* ball)
{
	this->ball = ball;
}


Pitch::~Pitch()
{
	if (this->localTeam != NULL)
		delete(this->localTeam);
	if (this->awayTeam != NULL)
		delete(this->awayTeam);
}
