#include <iostream>
#include <set>

using namespace std;

class Message;

class Folder{
    public:
        Folder():msg_set(){}
        ~Folder(){};
        void addMsg(Message *msg){
            msg_set.insert(msg);
        }
        void remMsg(Message *msg){
            set<Message*>::iterator itr = msg_set.find(msg);
            if (itr != msg_set.end())
                msg_set.erase(itr);
        }
        void printMsg();
    private:
        set<Message*> msg_set;
};


class Message{
    public:
        Message(const string &str = ""):
            content(str) {}
        Message(const Message&);
        Message& operator= (const Message&);
        ~Message();

        void save(Folder &fld) {
            cout << "save message:" << content << endl;
            fld.addMsg(this);
        }
        void remove(Folder &fld) {
            cout << "remove message:" << content << endl;
            fld.remMsg(this);
        }
        string& get_content(){return content;}
    private:
        string content;
        set<Folder*> folders;
        void put_Msg_in_Folders(const set<Folder*>&);
        void remove_Msg_from_Folders();
};

Message::Message(const Message &m)
{
    put_Msg_in_Folders(folders);
}

void Message::put_Msg_in_Folders(const set<Folder*> &rhs)
{
    for (set<Folder*>::const_iterator beg = rhs.begin();
            beg != rhs.end(); ++beg)
        (*beg)->addMsg(this);
}

Message& Message::operator=(const Message &rhs)
{
    if (&rhs != this)
    {
        remove_Msg_from_Folders(); //update existing Folders
        content = rhs.content;
        folders = rhs.folders;
        put_Msg_in_Folders(rhs.folders);
    }
    return *this;
}

void Message::remove_Msg_from_Folders()
{
    for (set<Folder*>::const_iterator beg = folders.begin();
            beg != folders.end(); ++beg)
        (*beg)->remMsg(this);
}

Message::~Message()
{
    remove_Msg_from_Folders();
}

void Folder::printMsg(){
    set<Message*>::const_iterator itr = msg_set.begin();
    for( ;itr != msg_set.end(); ++itr)
        cout << (*itr)->get_content() << endl;
}

int main(int argc,char **argv)
{
    Message msg1("hello");
    Message msg2("world");
    Folder fld1;
    msg1.save(fld1);
    msg2.save(fld1);
    fld1.printMsg();

    msg1.remove(fld1);
    fld1.printMsg();
    return 0;
}

