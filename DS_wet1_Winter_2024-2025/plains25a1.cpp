// You can edit anything you want in this file.
// However you need to implement all public Plains function, as provided below as a template

#include "plains25a1.h"


Plains::Plains()
{
    
}

Plains::~Plains()
{
    
}

StatusType Plains::add_herd(int herdId)
{
    if (nonEmptyHerds->find(herdId) != nullptr){
        //FIXME means herd already exists;
        return StatusType::FAILURE;
    }
    Herd newHerd(herdId);
    if (newHerd == nullptr){return StatusType::ALLOCATION_ERROR;} //FIXME FIX ME
    this->emptyHerds = emptyHerds->insert(newHerd); //FIXME compiller doesnt like that i didnt write herdmap yet and this is the source of this problem
    return *(this->emptyHerds->getReturnVal());
    //return StatusType::FAILURE;
}

StatusType Plains::remove_herd(int herdId)
{
    this->emptyHerds->deleteNode(herdId);
    return *(this->emptyHerds->getReturnVal());
}

StatusType Plains::add_horse(int horseId, int speed)
{
    Horse* verifyNewHorse = this->allHorses->find(horseId);
    if (verifyNewHorse != nullptr){
        //FIXME correct to appropriate status type
        return StatusType::FAILURE;
    }
    Horse newHorse(horseId,speed);
    if (newHorse == NULL){return StatusType::ALLOCATION_ERROR;} //FIXME FIX ME
    this->allHorses = this->allHorses->insert(newHorse);
    return *(this->allHorses->getReturnVal());
}

StatusType Plains::join_herd(int horseId, int herdId) //FUNCTION LOGIC INCOMPLETE
{
    Horse* sooson = this->allHorses->find(horseId);
    if (sooson == nullptr){return StatusType::FAILURE;}
    Herd* eder = this->emptyHerds->find(herdId);
    if (eder != nullptr){

    }
    //FIX ME - if Herd is in empty herds, remove it and insert into nonemptyherds , with the added horse
    if (eder == nullptr){eder = this->nonEmptyHerds->find(herdId);}
    if (eder == nullptr){return StatusType::FAILURE;}
    eder->addHorse(sooson); //TODO make addHorse function in Herd, basically insert a pointer to the horse inside the Herd's HorseMap, and update ammount of horses in the herd horses counter
    return StatusType::FAILURE;
}

StatusType Plains::follow(int horseId, int horseToFollowId)
{
    return StatusType::FAILURE;
}

StatusType Plains::leave_herd(int horseId)
{
    /**
     * pseudo -
     * find horse in allhorses
     * get the id of the herd from the horse
     * find the herd inside non empty herds and keep a pointer to it.
     * call function of leave herd from horse or whatever - still need to make it
     * if herd is now empty, remove it from non empty herds and add it back to empty herds
     */
    return StatusType::FAILURE;
}

output_t<int> Plains::get_speed(int horseId)
{
    return 0;
}

output_t<bool> Plains::leads(int horseId, int otherHorseId)
{
    return false;
}

output_t<bool> Plains::can_run_together(int herdId)
{
    return false;
}
