// You can edit anything you want in this file.
// However you need to implement all public Plains function, as provided below as a template

#include "plains25a1.h"


Plains::Plains()
{
    //how to do the initialisation list?
}

Plains::~Plains()
{
    delete allHorses;
    delete nonEmptyHerds;
    delete emptyHerds;
}

StatusType Plains::add_herd(int herdId)
{
    try {
    if (herdId <= 0){
        return StatusType::INVALID_INPUT;
    }
    if (nonEmptyHerds.get(herdId) != nullptr){
        return StatusType::FAILURE;
    }
    //TODO add check for if the herd exists in EmptyHerds
    Herd* newHerd = nullptr;
    try {
        newHerd = new Herd(herdId);
    } catch (...) {
        return StatusType::ALLOCATION_ERROR;
    }
    bool insertionWorked = emptyHerds.insert(*newHerd);
    return (insertionWorked)?(StatusType::SUCCESS):(StatusType::FAILURE);
    } catch (StatusType e){
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Plains::remove_herd(int herdId)
{
    bool operationWorked = emptyHerds.remove(herdId);
    return (operationWorked)?(StatusType::SUCCESS):(StatusType::FAILURE);
}

StatusType Plains::add_horse(int horseId, int speed)
{
    try {
    if (horseId <= 0){
        return StatusType::INVALID_INPUT;
    }
    if (speed <= 0){
        return StatusType::INVALID_INPUT;
    }
    Horse* newHorse = allHorses.get(horseId);
    if (newHorse != nullptr){
        return StatusType::FAILURE;
    }
    try {
        newHorse = new Horse(horseId,speed);
    } catch (...) {
        return StatusType::ALLOCATION_ERROR;
    }
    bool operationWorked = allHorses.insert(*newHorse);
    return (operationWorked)?(StatusType::SUCCESS):(StatusType::FAILURE);
    } catch (StatusType e){
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Plains::join_herd(int horseId, int herdId)
{
    try {
    if ((horseId <= 0)||(herdId <= 0)){return StatusType::INVALID_INPUT;}

    Horse* sooson = allHorses.get(horseId);

    if (sooson == nullptr){return StatusType::FAILURE;}

    Herd* eder = emptyHerds.get(herdId);
    if (eder != nullptr){
        emptyHerds.remove(herdId); //FIXME check that horse is not in a herd
        try {
            eder = new Herd(herdId);
        } catch (...){
            return StatusType::ALLOCATION_ERROR;
        }
        eder->add_horse(sooson);
        nonEmptyHerds.insert(*eder);
    }
    if (eder == nullptr){
        eder = nonEmptyHerds.get(herdId);
    }
    if (eder == nullptr){return StatusType::FAILURE;}
    eder->add_horse(sooson); //TODO make addHorse function in Herd, basically insert a pointer to the horse inside the Herd's HorseMap, and update ammount of horses in the herd horses counter
    return StatusType::SUCCESS;
    } catch (StatusType e){
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Plains::follow(int horseId, int horseToFollowId)
{
    try {
    if ((horseId == horseToFollowId)||(horseId <= 0)||(horseToFollowId <= 0)){
        return StatusType::INVALID_INPUT;
    }
    Horse* firstHorse = allHorses.get(horseId);
    if (firstHorse == nullptr){
        return StatusType::FAILURE;
    }
    Horse* secondHorse = allHorses.get(horseId);
    if (secondHorse == nullptr){
        return StatusType::FAILURE;
    }
    bool operationWorked = firstHorse->follow(secondHorse);
    return (operationWorked)?(StatusType::SUCCESS):(StatusType::FAILURE);
    } catch (StatusType e){
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Plains::leave_herd(int horseId)
{
    try {
    if (horseId <= 0){
        return StatusType::INVALID_INPUT;
    }
    Horse* horseToLeave = allHorses.get(horseId);
    if (horseToLeave == nullptr){
        return StatusType::FAILURE;
    }
    bool succes = horseToLeave->leaveHerd();
    return (succes)?(StatusType::SUCCESS):(StatusType::FAILURE);
    } catch (StatusType e){
        return StatusType::ALLOCATION_ERROR;
    }
}

output_t<int> Plains::get_speed(int horseId)
{
    try {
    if (horseId <= 0){
        return StatusType::INVALID_INPUT;
    }
    Horse* horseToSpeed = allHorses.get(horseId);
    if (horseToSpeed == nullptr){
        return StatusType::FAILURE;
    }
    int speed = horseToSpeed->getSpeed();
    return speed;
    } catch (StatusType e){
        return StatusType::ALLOCATION_ERROR;
    }
}

output_t<bool> Plains::leads(int horseId, int otherHorseId)
{
    try {
    if ((horseId <= 0)||(otherHorseId <= 0)){
        return StatusType::INVALID_INPUT;
    }
    Horse* firstHorse = allHorses.get(horseId);
    if (firstHorse == nullptr){
        return StatusType::FAILURE;
    }
    Horse* secondHorse = allHorses.get(horseId);
    if (secondHorse == nullptr){
        return StatusType::FAILURE;
    }
    int HerdIDfirst = firstHorse->getHerdID();
    int HerdIDsecond = secondHorse->getHerdID();
    if (HerdIDfirst != HerdIDsecond){
        return false;
    }
    Herd* herd = nonEmptyHerds.get(HerdIDfirst);
    return herd->leads(horseId, otherHorseId);
    } catch (StatusType e){
        return StatusType::ALLOCATION_ERROR;
    }
}

output_t<bool> Plains::can_run_together(int herdId)
{
    try {
    if (herdId <= 0){
        return StatusType::INVALID_INPUT;
    }
    Herd* herd = nonEmptyHerds.get(herdId);
    if (herd == nullptr){
        return StatusType::FAILURE;
    }
    return herd->can_run_together();
    } catch (StatusType e){
        return StatusType::ALLOCATION_ERROR;
    }
}
