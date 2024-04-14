#include <iostream>

template <class T>
struct List
{
    T data;
    List* next;
};

template <class T>
void Print(List<T>* list)
{
    List<T>* current = list;
    while (current != NULL)
    {
        std::cout << current->data << ",";
        current = current->next;
    }

    std::cout << std::endl;
}

template <class T>
List<T>* reverseList(List<T>* list)
{
    List<T>* prev = NULL;
    List<T>* next = NULL;

    while (list != NULL)
    {
        next = list->next;
        list->next = prev;
        prev = list;
        list = next;
    }    

    return prev;
}

template <class T>
List<T>* add_front(T element, List<T>* list)
{
    List<T>* res = new List<T>;
    (*res).data = element;
  
    res->next = list;
    return res;    
}

void removeLastElement(List<int>* list)
{
    while (list->next->next != NULL)
    {
        list = list->next;
    }
    
    list->next = NULL;

    delete list->next;
}

template <class T>
List<T>* append(List<T>* list1, List<T>* list2)
{
    if (list1 == NULL)
    {
        return list2;
    }

    List<T>* res = list1;

    while (list1->next != NULL)
    {
        list1 = list1->next;
    }

    list1->next = list2;

    return res;
}


List<std::string>* tokenize(std::string& str)
{
    std::string word = "";
    List<std::string>* res = NULL;

    for (auto& a : str)
    {
	    if (a == ' ')
	    {
	        if (word != "")
	        {
		        res = add_front(word, res);
		        word = "";
	        }
	    }
	
	    if (a != ' ')
	    {
	        word += a;
	    }
    }
    
    res = add_front(word, res);

    return res;
}

//1 2 3 + 4 * +
//stack: 21

int calculate(List<std::string>* token)
{

}


int main()
{
    std::string s = "45          67 +";
    List<std::string>* r = NULL;
    r = tokenize(s);
    r = reverseList(r);
    Print(r);

    return 0;
}

//1*3+4
//+ 1 * 3 4
//4 3 1 * +

//(4+2)*5

//4 2 * 5 +

//(1+3)*(5+2)

//1 3 + 5 2 + * 

//(4+5+7)*(6-1)
//4 5 + 7 + 6 1 - *

//токенизация   "45  67 +"

//-> "45","67","+"
