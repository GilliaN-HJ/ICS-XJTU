#include "resolve.h"

#include <iostream>

#define FOUND_ALL_DEF 0
#define MULTI_DEF 1
#define NO_DEF 2

std::string errSymName;

int callResolveSymbols(std::vector<ObjectFile> &allObjects);

void resolveSymbols(std::vector<ObjectFile> &allObjects) {
    int ret = callResolveSymbols(allObjects);
    if (ret == MULTI_DEF) {
        std::cerr << "multiple definition for symbol " << errSymName << std::endl;
        abort();
    } else if (ret == NO_DEF) {
        std::cerr << "undefined reference for symbol " << errSymName << std::endl;
        abort();
    }
}

/* bind each undefined reference (reloc entry) to the exact valid symbol table entry
 * Throw correct errors when a reference is not bound to definition,
 * or there is more than one definition.
 */
int callResolveSymbols(std::vector<ObjectFile> &allObjects)
{
    /* Your code here */
    // if found multiple definition, set the errSymName to problematic symbol name and return MULTIDEF;
    // if no definition is found, set the errSymName to problematic symbol name and return NODEF;
    for (auto &obj : allObjects) {
        for (auto &re : obj.relocTable) {
            int flag = 0;
            int count = 0;
            Symbol * temp_strong, *temp_weak;
            
            for (auto &o1 : allObjects){
                for (auto &s1 : o1.symbolTable){
                    //strong
                    if (s1.name == re.sym->name && s1.bind == STB_GLOBAL && (s1.index != SHN_UNDEF && s1.index != SHN_COMMON)) {
                        flag = 1;
                        count++;
                        temp_strong = &s1;
                        //std::cout << re.sym->name << " :strong" <<std::endl;
                        //std::cout << re.sym << " :strong" <<std::endl;
                        
                    }
                    
                    //weak
                    //test4
                    else if (s1.name == re.sym->name && s1.bind == STB_GLOBAL && s1.index == SHN_COMMON) { 
                        // TODO: find the same-name strong symbol
                        for (auto &o2 : allObjects){
                            for (auto &s2 : o2.symbolTable){
                                if (s2.name == re.sym->name && s2.bind == STB_GLOBAL && (s2.index != SHN_UNDEF && s2.index != SHN_COMMON)){
                                    //std::cout << s2.name << " strong" <<std::endl;
                                    re.sym = &s2;
                                    //std::cout << re.sym << " after" <<std::endl;
                                    break;
                                }

                                //test5
                                else if (s2.name == re.sym->name && s2.bind == STB_GLOBAL && s2.index == SHN_COMMON){
                                    re.sym = &s2;
                                    flag = 2;
                                }
                            }
                        }  
                    }    
                }
            } 
            
            //test2
            if (!flag) {
                errSymName = re.sym->name;
                return NO_DEF;
            } 

            //test3
            if (count > 1){
                errSymName = re.sym->name;
                return MULTI_DEF;
            }
        }
        
    }
    return FOUND_ALL_DEF;
}
