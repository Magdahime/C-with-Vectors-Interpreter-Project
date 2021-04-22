#include "helpers/positioner.hpp"

void Positioner::nextChar()
{
    this->character += 1;
    this->absolutePosition +=1;

}
void Positioner::nextLine()
{
    this->line += 1;
    this->character = 0;
}