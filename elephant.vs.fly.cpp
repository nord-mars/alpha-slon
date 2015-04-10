#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <list>

using namespace std;

bool file_read(const char *filename, list<wstring> *l);
template<typename T> void contaner_print(T *s);
bool r_find(list<wstring> *lst, wstring start, wstring finish, int dif, int size);

int main()
{
    // locale
    std::setlocale(LC_ALL, "ru_RU.utf8");
    std::locale::global(std::locale(""));
    std::wcout.imbue(std::locale());
    std::wcerr.imbue(std::locale());
    std::wclog.imbue(std::locale());
    std::ios::sync_with_stdio(false);

    // read from file to set<wstring> (set kill retrain)
    const char* fileIns = "start.dat";
    const char* fileDic = "dictionary.dat";

    list<wstring> sta;
    list<wstring> dic;

//    wcout << "read date from file\n";
    if( false == file_read(fileIns, &sta) ) return -1;
    if( false == file_read(fileDic, &dic) ) return -1;

    // устраняем ошибку в постановке задачи предполагающую
    // что слова в начальном диапазоне отсортированы правильно
    sta.sort();
    // pointer to the start and end word 
    list<wstring>::const_iterator it_first = sta.begin();
    list<wstring>::const_iterator it_last  = sta.end();

    // print file info
//    wcout << "diapason etalon:\n"; contaner_print(&sta);
//    wcout << "dictionary:\n";      contaner_print(&dic);

    // remove long/short word
    uint w_size=it_first->length();
    auto condition=[&w_size](wstring s){ return s.size() == w_size ? false : true; };  
    auto it_remove = remove_if( dic.begin(), dic.end(),
                           condition 
                         );
    dic.erase(it_remove, dic.end());

//    wcout << "after erase long/short word != " << w_size << ":\n";
//    contaner_print(&dic);


    // remove repeats
    dic.sort();
    dic.unique();

//    wcout << "after sort and unique:\n";
//    contaner_print(&dic);

    // The elephant
    wcout << *sta.begin() << " - start!\n";
    if ( false == r_find(&dic, *sta.begin(), *sta.rbegin(), 1, dic.size()) )
        wcout << *sta.rbegin() << " - the chain is bed!\n";
    else
        wcout << *sta.rbegin() << " - the chain is good!\n";

    return 0;
}
bool file_read(const char *filename, list<wstring> *l)
{
     wstring wstr;
     wifstream ifs;
     ifs.open(filename, ios_base::in);
     if(!ifs.is_open()){
         cout << "Can't open filename: " << filename << endl;
         return false;
     }
     while(!ifs.eof())
     {
         getline(ifs, wstr);
         l->emplace(l->begin(), wstr);
     }
     ifs.close();
     return true;
}
template<typename T>
void contaner_print(T *s)
{
    for(auto w: *s)
    {
        wcout << "  " << w << endl;
    }
}
bool r_find(list<wstring> *lst, wstring start, wstring finish, int dif, int size)
{
    if( start.size() != finish.size() ){
        wcerr << "!!!ERROR the size is different!!!\n";
        return false;
    }
    if( size == 0 ){
        wcerr <<  "!!!END not found!!!\n";
        return false;
    }
    if( start == finish ) {
        return true; // We found!
    }

    uint c, f;
    auto it=lst->begin();
    for(; it != lst->end(); it++)
    {
        c=0;
        f=0;
        for(uint i=0; i < (*it).size(); i++){
            if( (*it)[i] == start[i] ){
                c++;
            }
            if( (*it)[i] == finish[i]   ){
                f++;
            }
        }
        if( f == (finish.size() - dif) ){
            wcout << *it << endl;
            return true; // The next is will be our result!
        }
        if( c == (start.size() - dif) ){
            start = *it;
            wcout << start << endl;
            it = lst->erase(it); 
            size = lst->size();
            return r_find(lst, start, finish, dif, size);
        }
   }
   return false;
}
