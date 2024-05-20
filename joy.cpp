#include <iostream>

template <class T>
struct List
{
    T data;
    List* next;
};

List<int>* reverseList(List<int>* list)
{
    List<int>* prev = NULL;
    List<int>* next = NULL;

    while (list != NULL)
    {
        next = list->next;
        list->next = prev;
        prev = list;
        list = next;
    }    

    return prev;
}

//почитал, и чтобы не возникало ошибок с new, лучше использовать variant
/* enum ValueType 
{ 
    DATA, 
    SUBLIST 
};

template <class T>
struct ListSub;

template <class T>
using Sublist = std::variant<T, ListSub<T>*>;

template <class T>
struct ListSub
{
    Sublist<T> sublist;
    ValueType type;
    ListSub* next;
};
 */


enum ValueType 
{ 
    DATA, 
    SUBLIST 
};

template <class T>
struct ListSub
{
    union Sublist
    {
        T data;
        ListSub* sub;
    } sublist;

    ValueType type;
    ListSub* next;
};

template <class T>
ListSub<T>* reverseList(ListSub<T>* list)
{
    ListSub<T>* prev = NULL;
    ListSub<T>* current = list;
    ListSub<T>* next = NULL;
    while (current != NULL) 
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    
    return prev;
}

template <class T>
ListSub<T>* Pop(ListSub<T>* stack)
{
    ListSub<T>* temp = stack->next;
    //free(stack);
    return stack = temp;
}

template <class T>
ListSub<T>* addData(ListSub<T>* list, T data)
{
    //ListSub<T>* res = new ListSub<T>; можно использовать, если взять variant
    ListSub<T>* res = (ListSub<T>*) malloc(sizeof(ListSub<T>));
    if (res == NULL)
    {
        std::cerr << "Ошибка выделения памяти\n";
        return NULL;
    }
    res->type = DATA;
    res->sublist.data = data;
    res->next = list;
    return res;
}

template <class T>
ListSub<T>* addSublist(ListSub<T>*& list, ListSub<T>* sublist)
{
    //ListSub<T>* res = new ListSub<T>; можно использовать, если взять variant
    ListSub<T>* res = (ListSub<T>*) malloc(sizeof(ListSub<T>));
    if (res == NULL)
    {
        std::cerr << "Ошибка выделения памяти\n";
        return NULL;
    }
    res->type = SUBLIST;
    res->sublist.sub = sublist;
    res->next = list;
    return res;
}

template <class T>
T first(ListSub<T>* list) 
{
    if (list == nullptr) 
    {
        throw std::runtime_error("List is empty");
    }
    if (list->type == DATA) 
    {
        return list->sublist.data;
    } 
    else if (list->type == SUBLIST) 
    {
        return first(list->sublist.sub);
    } 
    else 
    {
        throw std::runtime_error("Unknown type");
    }
}

template <class T>
void removeLastElement(ListSub<T>* list)
{
    while (list->next->next != NULL)
    {
        list = list->next;
    }
    
    list->next = NULL;
    //free(list->next);
}

template <class T>
ListSub<T>* cons(ListSub<T>* list)
{
    std::string res = list->next->sublist.data;

    list->next = Pop(list->next);
    list->sublist.sub = addData(list->sublist.sub, res);

    return list;
}

template <class T>
ListSub<T>* rest(ListSub<T>* list)
{
    if (list != NULL)
    {
        list->sublist.sub = list->sublist.sub->next;
        return list;
    }

    return NULL;
}

template <class T>
ListSub<T>* dup(ListSub<T>* list)
{
    if (list == NULL)
    {
        return NULL;
    }

    if (list->type == ValueType::DATA)
    {
        return addData(list, list->sublist.data);
    }
    else if (list->type == ValueType::SUBLIST)
    {
        ListSub<T>* data = (ListSub<T>*) malloc(sizeof(ListSub<T>));

        data->type = ValueType::SUBLIST;

        data->sublist.sub = dup(list->sublist.sub);

        ListSub<T>* current = list;
        while (current != NULL)
        {
            if (current->type == ValueType::DATA)
            {
                data->sublist.data = current->sublist.data;
            }
            else if (current->type == ValueType::SUBLIST)
            {
                data->sublist.sub = current->sublist.sub; 
            }

            current = current->next;
        }
        
        return addSublist(data, data->sublist.sub);
    }

    return NULL;
}

template <class T>
ListSub<T>* swap(ListSub<T>* list)
{
    if (list == NULL || list->next == NULL)
    {
        return list;
    }

    ListSub<T>* tmp = NULL;

    if (list->type == ValueType::DATA)
    {
        tmp = addData(tmp, list->sublist.data);
        tmp = addData(tmp, list->next->sublist.data);

        list = Pop(list);
        list = Pop(list);

        list = addData(list, tmp->next->sublist.data);
        list = addData(list, tmp->sublist.data);

        return list;
    }
    else if (list->type == ValueType::SUBLIST)
    {
        tmp = addSublist(tmp, list->sublist.sub);
        tmp = addSublist(tmp, list->next->sublist.sub);

        list = Pop(list);
        list = Pop(list);

        list = addSublist(list, tmp->next->sublist.sub);
        list = addSublist(list, tmp->sublist.sub);

        return list;
    }

    return NULL;
}

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
void PrintSub(ListSub<T>* list)
{
    while (list != NULL) 
    {
        if (list->type == ValueType::DATA) 
        {
            std::cout << list->sublist.data;
            if (list->next != NULL)
            {
                std::cout << ",";
            }
        } 
        else if (list->type == ValueType::SUBLIST) 
        {
            std::cout << "[";
            PrintSub(list->sublist.sub); 
            std::cout << "]";
            if (list->next != NULL)
            {
                std::cout << ",";
            }
        }
        else
        {
            std::cout << "Error in PrintSub" << std::endl;
        }

        list = list->next;
    }
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

ListSub<std::string>* tokenize(std::string& str)
{
    std::string sign = "";
    ListSub<std::string>* res = NULL;

    for (char& a : str) 
    {
        if (a == ' ' || a == '[' || a == ']') 
        {
            if (!sign.empty()) 
            {
                res = addData(res, sign);
                sign = "";
            }
            
            if (a == '[' || a == ']') 
            {
                std::string bracket(1, a);
                res = addData(res, bracket);
            }
        } 
        else 
        {
            sign += a;
        }
    }

    if (!sign.empty()) 
    {
        res = addData(res, sign);
    }

    return reverseList(res);
}

template <class T>
int len(ListSub<T>* list)
{
    int res = 0;

    while (list != NULL)
    {
        list = list->next;
        ++res;
    }

    return res;
}

template <class T>
void removeList(List<T>* list)
{
    List<T>* current = list;
    List<T>* nxt;

    while (current != NULL)
    {
        nxt = current->next;
        delete current;
        current = nxt;
    }
}

template <class T>
ListSub<T>* removeFirstElement(ListSub<T>* list)
{
    /* if (list == NULL) 
    {
        return;
    }

    ListSub<T>* temp = list; */
    return list = list->next;
    //free(temp);
}

ListSub<std::string>* Parser(ListSub<std::string>* list)
{
    ListSub<std::string>* stack = NULL;

    while (list != NULL)
    {
	    if (list->sublist.data == "]")
	    {
	        ListSub<std::string>* tmp = NULL;

	        while (stack != NULL)
	        {   
		        if (stack->type == ValueType::DATA && stack->sublist.data == "[")
		        {
		            stack = removeFirstElement(stack);
		            break;
		        }

                if (stack->type == ValueType::SUBLIST)
                {
                    tmp = addSublist(tmp, stack->sublist.sub);
                }// If there are less than 2 elements, nothing to swap

                if (stack->type == ValueType::DATA)
                {
                    tmp = addData(tmp, stack->sublist.data);
                }
                
		        stack = stack->next;
	        }

	        stack = addSublist(stack, tmp);
	    }
        else
        {
            stack = addData(stack, list->sublist.data);
        }

	    list = list->next;
    }   

    return reverseList(stack);
}

bool isNumber(const std::string& s)
{
    for (char c : s)
    {
        if (!std::isdigit(c))
            return false;
    }

    return !s.empty();
}


ListSub<std::string>* calculate(ListSub<std::string>* token)
{
    ListSub<std::string>* stack = NULL;

    while (token != NULL)
    {
        if (token->type == ValueType::DATA)
        {
            if (token->sublist.data == "first" && stack != NULL)
            {
                std::string tmp = first(reverseList(stack));

                stack = Pop(stack);
                stack = addData(stack, tmp);
            }
            else if (token->sublist.data == "rest" && stack != NULL)
            {
                stack = rest(stack);
            }
            else if (token->sublist.data == "cons" && stack != NULL)
            {
                stack = cons(stack);
            }
            else if (token->sublist.data == "dup" && stack != NULL)
            {
                stack = dup(stack);
            }
            else if (token->sublist.data == "swap" && stack != NULL)
            {
                stack = swap(stack);
            }
            else if (token->sublist.data == "+" && stack != NULL && stack->next != NULL)
            {
                int r = 0;

                r = std::stoi(stack->sublist.data) + std::stoi(stack->next->sublist.data);

                stack = Pop(stack);
                stack = Pop(stack);

                stack = addData(stack, std::to_string(r));
            }
            else if (token->sublist.data == "-" && stack != NULL && stack->next != NULL)
            {
                int r = 0;

                r = std::stoi(stack->next->sublist.data) - std::stoi(stack->sublist.data);

                stack = Pop(stack);
                stack = Pop(stack);

                stack = addData(stack, std::to_string(r));
            }
            else if (token->sublist.data == "*" && stack != NULL && stack->next != NULL)
            {
                int r = 0;

                r = std::stoi(stack->sublist.data) * std::stoi(stack->next->sublist.data);

                stack = Pop(stack);
                stack = Pop(stack);

                stack = addData(stack, std::to_string(r));
            }
            else if (token->sublist.data == "/" && stack != NULL && stack->next != NULL)
            {
                int r = 0;

                r = std::stoi(stack->sublist.data) / std::stoi(stack->next->sublist.data);

                stack = Pop(stack);
                stack = Pop(stack);

                stack = addData(stack, std::to_string(r));
            }
            else if (isNumber(token->sublist.data))
            {
                stack = addData(stack, token->sublist.data);
            }
        }
        else if (token->type == ValueType::SUBLIST)
        {
            stack = addSublist(stack, token->sublist.sub);
        }
        token = token->next;
    }

    return stack;
}


int main()
{
    std::string str_1 = "[1 2 3] [4 5] [23 47] swap";
    ListSub<std::string>* res = tokenize(str_1);
    res = Parser(res);

    PrintSub(res);
    std::cout << std::endl;

    ListSub<std::string>* pr= calculate(res);

    PrintSub(reverseList(pr));
    std::cout << std::endl;

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
