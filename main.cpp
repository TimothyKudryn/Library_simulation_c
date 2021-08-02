// COUTS are also good comments. They could help you figure out what the code is doing
#include <iostream>
#include <string>
#include <list>
#include <algorithm>

using namespace std;

class Patron;        // forward declaration;

class Book {
public:
    Book() {
        patron = NULL;
        numCopies = 1;
        numCheckout = 0;
    }
    bool operator== (const Book& bk) const {
        return strcmp(title, bk.title) == 0;
    }
    /*int get_numCopies(){
        return numCopies;
    }*/

private:
    char *title;
    Patron *patron;
    int numCopies;
    int numCheckout;
    ostream& printBook(ostream&) const;
    friend ostream& operator<< (ostream& out, const Book& bk) { //overloaded insertion
        return bk.printBook(out);
    }
    friend class CheckedOutBook;
    friend Patron;
    friend void includeBook();
    friend void checkOutBook();
    friend void returnBook();
};

class Author {
public:
    Author() {
    }
    bool operator== (const Author& ar) const {
        return strcmp(name, ar.name) == NULL; // not a bool, should it be NULL?
    }
private:
    char *name;
    list<Book> books;
    ostream& printAuthor(ostream&) const;
    friend ostream& operator<< (ostream& out, const Author& ar) {
        return ar.printAuthor(out);
    }
    friend void includeBook();
    friend void checkOutBook();
    friend void returnBook();
    friend class CheckedOutBook;
    friend Patron;
};

class CheckedOutBook {
public:
    CheckedOutBook(list<Author>::iterator ar, list<Book>::iterator bk) {
        author = ar;
        book = bk;
    }
    bool operator== (const CheckedOutBook& bk) const {
        return strcmp(author->name, bk.author->name) == 0 &&
               strcmp(book->title, bk.book->title) == 0;
    }
private:
    list<Author>::iterator author;
    list<Book>::iterator book;
    friend void checkOutBook();
    friend void returnBook();
    friend Patron;
};

class Patron {
public:
    Patron() {
    }
    bool operator== (const Patron& pn) const {
        return strcmp(name, pn.name) == 0;
    }
private:
    char *name;
    list<CheckedOutBook> books;
    ostream& printPatron(ostream&) const;
    friend ostream& operator<< (ostream& out, const Patron& pn) {
        return pn.printPatron(out);
    }
    friend void checkOutBook();
    friend void returnBook();
    friend Book;
};

list<Author> catalog['Z' + 1];
list<Patron> people['Z' + 1];

ostream& Author::printAuthor(ostream& out) const {
    out << name << endl;
    list<Book>::const_iterator ref = books.begin();
    for (; ref != books.end(); ref++)
        out << *ref; // overloaded <<
    return out;

    //overloaded << operator to make it easy to "cout" the authors and there books
}

ostream& Book::printBook(ostream& out) const {
    out << "    * " << title;
    out << "  copies owned: " << numCopies << "  copies lent out: " << numCheckout << endl;// overloaded <<

    out << endl;
    return out;
    //function to output who a certain book is checked out too
}

ostream& Patron::printPatron(ostream& out) const {
    out << name;
    if (!books.empty()) {
        out << " has the following books:\n";
        list<CheckedOutBook>::const_iterator bk = books.begin();
        for (; bk != books.end(); bk++)
            out << "    * " << bk->author->name << ", "
                << bk->book->title << endl;
    }
    else out << " has no books\n";

    return out;
    //function that outputs the name of the person who is using the library (and the book he has checked out)
    // (in format (author, book title)
}

template<class T>
ostream& operator<< (ostream& out, const list<T>& lst) {
    for (typename list<T>::const_iterator ref = lst.begin(); ref != lst.end(); ref++)
        out << *ref; // overloaded <<
    return out;
    // overloaded << used to print records from any list that it is called on. 
}

char* getString(const char *msg) {
    char s[82], i, *destin;
    cout << msg;
    cin.get(s, 80);
    while (cin.get(s[81]) && s[81] != '\n');  // discard overflowing
    destin = new char[strlen(s) + 1];           // characters;
    for (i = 0; destin[i] = toupper(s[i]); i++); //toupper uppercases all elements of string (if equvialent to exists)
    return destin;
    //this function takes in all inputs from the user. After the user selects what action he wants to take,
    //this function will ask the authors name, the books title or the patrons name and save it into a char str
}

void status() {
    int i = 0;
    cout << "Library has the following books:\n\n";
    for (i = 'A'; i <= 'Z'; i++)
        if (!catalog[i].empty()) {
            cout << catalog[i];
        }

    cout << "\nThe following people are using the library:\n\n";
    for (i = 'A'; i <= 'Z'; i++)
        if (!people[i].empty())
            cout << people[i];

    //tells the user what the status of the books at any given time.
    //calls functions from author and book class
    //uses overloaded operator << to make printing easy.
}

void includeBook() {
    //this function adds a new book to the library.
    /*
        Ask's user for name of the author and the title of the book
        Checks if the book is already in the inventory.
        It does this by, first checking whether the author is in the list of authors
        if not, than goes ahead and pushes the book onto the list automatically, and then pushes the
        author onto a the list of authors.
        If already author exists, than it iterates through the books of the given author and looks
        for the particular book. if it exist, than it tells the user that they can't add the same book
        to the list twice
        Otherwise it adds the book to the authors list of books.
     */
    Author newAuthor;
    Book newBook;
    newAuthor.name = getString("Enter author's name: ");
    newBook.title = getString("Enter the title of the book: ");
    list<Author>::iterator authorRef;
    list<Book> :: iterator bookRef;
    list<Book> :: iterator bookRef2;
    authorRef = find(catalog[newAuthor.name[0]].begin(), catalog[newAuthor.name[0]].end(), newAuthor);

    if (authorRef == catalog[newAuthor.name[0]].end()) {
        //case new author
        newAuthor.books.push_front(newBook);
        catalog[newAuthor.name[0]].push_front(newAuthor);
    }
    else {
        // case old author (author already in list)
        bookRef = find(authorRef->books.begin(), authorRef->books.end(), newBook);
        if(bookRef == authorRef -> books.end()) {
            authorRef->books.push_front(newBook);
            // now new book is at start of list
        }
        else{
            bookRef2 = authorRef->books.begin(); //created new iterator because of compiler errors
            bookRef-> numCopies ++;
        }

    }

}

void checkOutBook() {
    /*
        This function checks out a book, by asking user for the author's name and the book title
        If there is no matching author or book to the ones the user entered, the program will
        notify the user and then boot him back to the main menu of actions he could take
        It determines weather or not the book exists by iterating through the list in much the same fashion
        as include book does
    */
    Patron patron;
    Author author;
    Book book;
    list<Author>::iterator authorRef;
    list<Book>::iterator bookRef;
    patron.name = getString("Enter patron's name: ");
    //looking for author name
    author.name = getString("Enter author's name: ");
    authorRef = find(catalog[author.name[0]].begin(), catalog[author.name[0]].end(), author);
    while(true){
        //break points at any given point are there to make sure you don't waste time if: line 243...
        if (authorRef == catalog[author.name[0]].end()) {
            cout << "The library doesn't have any books by this author" << endl;
            break; // author doesn't exist. (can't check out)
        }


        book.title = getString("Enter the title of the book: ");
        bookRef = find((*authorRef).books.begin(), (*authorRef).books.end(), book);
        if (bookRef == (*authorRef).books.end()) {
            cout << "The library does not have this book by " << author.name << endl;
            break; // no book with this title and author. (can't check out)
        }




        if(bookRef->numCopies > bookRef -> numCheckout){
            list<Patron>::iterator patronRef;
            patronRef = find(people[patron.name[0]].begin(), people[patron.name[0]].end(), patron);
            CheckedOutBook checkedOutBook(authorRef, bookRef);

            if (patronRef == people[patron.name[0]].end()) {// a new patron

                patron.books.push_front(checkedOutBook);    // in the library;
                people[patron.name[0]].push_front(patron);
                (*bookRef).patron = &*people[patron.name[0]].begin();

            }
            else {
                if (bookRef->patron == &*patronRef) {
                    cout << "Patron may no check out multiple copies of the same book at the same time" << endl;
                    break; // can't checkout multiplt copies at the same time.
                }
                else {
                    if (bookRef->numCopies >= bookRef->numCheckout) {
                        cout << "No available copies of this book." << endl;

                    }
                    else {
                        (*patronRef).books.push_front(checkedOutBook);
                        (*bookRef).patron = &*patronRef;
                    }
                }
            }
            bookRef->numCheckout++;
        }
        else cout << "there are no available copies of this book." << endl;
        break;

    }
}

void returnBook() {
    /*
        returns a book back to the library
        if patron isn't in patron list or the book or author aren't in the patron's inventory then the program will
        notify the user and then boot him back to the main menu of actions he could take.
        The program checks for the book by iterating through patrons inventory list.
    */
    Patron patron;
    Book book;
    Author author;

    //iterators for iterating through list of patrons, list of patrons books and authors
    list<Patron>::iterator patronRef;
    list<Book>::iterator bookRef;
    list<Author>::iterator authorRef;
    while (true) {
        //break points at any given postion are there to make sure no time is wasted if: go to line 315...
        patron.name = getString("Enter patron's name: ");
        patronRef = find(people[patron.name[0]].begin(),
                         people[patron.name[0]].end(), patron);
        if (patronRef == people[patron.name[0]].end()) {
            cout << "Patron isn't using the library\n";
            break; // said patron not using library
        }
        author.name = getString("Enter author's name: ");
        authorRef = find(catalog[author.name[0]].begin(), catalog[author.name[0]].end(), author);
        if (authorRef == catalog[author.name[0]].end()) {
            cout << " Patron has no books checked out by this author\n";
            break; //author not in patrons inventory list
        }
        book.title = getString("Enter the title of the book: ");
        bookRef = find((*authorRef).books.begin(), (*authorRef).books.end(), book);
        if (bookRef == (*authorRef).books.end()) {
            cout << "Patron can not return a book that he/she haven't checked out." << endl;
            //Book with this title and author aren't in the patrons inventory.
            break;
        }
        if (bookRef->patron == &*patronRef) {
            CheckedOutBook checkedOutBook(authorRef, bookRef);
            (*bookRef).patron = NULL;
            (*patronRef).books.remove(checkedOutBook);
            bookRef->numCheckout--;
        }
        else
            cout << "This user can not return a book that he/she hasn't checked out." << endl;
        break;
    }
}

int menu() {
    int option;
    cout << "\nEnter one of the following options:\n"
         << "1. Include a book in the catalog\n2. Check out a book\n"
         << "3. Return a book\n4. Status\n5. Exit\n"
         << "Your option? ";
    cin >> option;
    cin.get();         // discard '\n';
    return option;
    //menu function that receives inputs from the user and its results dictate what option
    // of the switch case in main to use
}

int main() {
    while (true)
        switch (menu()) {
            case 1: includeBook();  break;
            case 2: checkOutBook(); break;
            case 3: returnBook();   break;
            case 4: status();       break;
            case 5: return 0;
            default: cout << "Wrong option, try again: ";
        }
    return 0;
}
