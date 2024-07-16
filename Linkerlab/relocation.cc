#include "relocation.h"

#include <sys/mman.h>

void handleRela(std::vector<ObjectFile> &allObject, ObjectFile &mergedObject, bool isPIE)
{
    /* When there is more than 1 objects, 
     * you need to adjust the offset of each RelocEntry
     */
    /* Your code here */
    //test4 & test5
    //if (sizeof(allObject) > sizeof(allObject[0])){
        int accu = 0;
        for (auto &obj : allObject){
            
            for (auto &re : obj.relocTable){
                re.offset = re.offset + accu;
            }
            accu += obj.sections[".text"].size;
        }
    //}

    /* in PIE executables, user code starts at 0xe9 by .text section */
    /* in non-PIE executables, user code starts at 0xe6 by .text section */
    uint64_t userCodeStart = isPIE ? 0xe9 : 0xe6;
    uint64_t textOff = mergedObject.sections[".text"].off + userCodeStart;
    uint64_t textAddr = mergedObject.sections[".text"].addr + userCodeStart;

    /* Your code here */
    //test 0 
    for (auto &obj : allObject) {
        for (auto &re : obj.relocTable) {
            if (isPIE) { //re.type == 2 
                uint64_t addr = re.offset + textOff;
                int valueToFill = re.sym->value - (re.offset + textAddr) + re.addend;
                *reinterpret_cast<int *>(addr + (uint64_t)mergedObject.baseAddr) = valueToFill;
            }

            //test1
            else if (!isPIE){
                if (re.type == 4){ //why 4 ? where 3 ...
                    uint64_t addr = re.offset + textOff;
                    int valueToFill = re.sym->value - (re.offset + textAddr) + re.addend;
                    *reinterpret_cast<int *>(addr + (uint64_t)mergedObject.baseAddr) = valueToFill;
                }
                else{
                    uint64_t addr = re.offset + textOff;
                    int valueToFill = re.sym->value + re.addend;
                    *reinterpret_cast<int *>(addr + (uint64_t)mergedObject.baseAddr) = valueToFill;
                }
                
            }
        }
    }
}