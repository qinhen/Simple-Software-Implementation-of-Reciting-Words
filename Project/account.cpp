#ifndef ACCOUNT_CPP
#define ACCOUNT_CPP
#include "account.h"
#include "MyLevel4WordBook.h"
//@author:��
Md5Encode Account::encode_obj;

using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;

void str_copy(char*destination, char*source)
{
    for (int i = 0; i < ACCOUNT_NAME; i++)
    {
        destination[i] = source[i];
    }
}

Account::Account(char id, string name, string md) :
    vid(id),
    md5(md),
    na(name)
{
}

Account::~Account()
{

}

Account_running::Account_running(Account ac) :
    Account(ac)
{
    int a = vid;
    wv_undone = new vector<Wordnode>*[4];
    for (int i = 0; i < 4; i++)
    {
        wv_undone[i] = new vector<Wordnode>;
    }
    string file_name = string(to_string(a)) + string(".dat");
    ifstream ifs(file_name.c_str(), ios_base::binary);
    char ch = ifs.get();
    if (!ifs.is_open() || ifs.bad() || (ifs.eof()))
        return;
    ifs.putback(ch);
    short* id = new short;
    char* st = new char;
    char* ti = new char;
    short* da = new short;
    bool ifs_ok = true;
    for (; ifs&&ifs_ok;)
    {
        char ch = ifs.get();
        if (!ifs.is_open() || ifs.bad() || (ifs.eof()))
        {
            ifs_ok = false;
            break;
        }
        ifs.putback(ch);
        ifs.read((char*)id, 2);
        ifs.read((char*)st, 1);
        ifs.read((char*)ti, 1);
        ifs.read((char*)da, 2);
        Date d(*da);
        Wordnode w(*id, *st, *ti, d);
        switch (*ti)
        {
        case Wordnode::none:wl_new.addword(w); break;
        case Wordnode::first:wv_undone[0]->push_back(w); break;
        case Wordnode::second:wv_undone[1]->push_back(w); break;
        case Wordnode::third:wv_undone[2]->push_back(w); break;
        case Wordnode::fourth:wv_undone[3]->push_back(w); break;
        case Wordnode::fifth:wl_done.addword(*id, *st, *ti, *da); break;
        }
    }
    ifs.close();
    delete id, st, ti, da;
}

Account_running::~Account_running()
{
    int b = vid;
    string file_name = string(to_string(b)) + string(".dat");
    ofstream ofs(file_name.c_str(), ios_base::binary);
    Wordnode w;
    for (; wl_done.count_total() != 0;)
    {
        w = wl_done.pop();
        wl_done.delete_node(0);
        ofs.write((char*)&w.id, 2);
        ofs.write((char*)&w.st, 1);
        ofs.write((char*)&w.ti, 1);
        ofs.write((char*)&w.da, 2);
    }
    for (; wl_new.count_total() != 0;)
    {
        w = wl_new.pop();
        wl_new.delete_node(0);
        ofs.write((char*)&w.id, 2);
        ofs.write((char*)&w.st, 1);
        ofs.write((char*)&w.ti, 1);
        ofs.write((char*)&w.da, 2);
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = (*(wv_undone[i])).size(); j > 0; j--)
        {
            w = (*(wv_undone[i])).back();
            (*(wv_undone[i])).pop_back();
            ofs.write((char*)&w.id, 2);
            ofs.write((char*)&w.st, 1);
            ofs.write((char*)&w.ti, 1);
            ofs.write((char*)&w.da, 2);
        }
    }
    ofs.close();
    for (int i = 0; i < 4; i++)
    {
        delete wv_undone[i];
    }
    delete[]wv_undone;



    string fn = string(to_string(b)) + string("1.dat");//���ÿ������
    if (wl_daily.count_total() <= 0)
    {
        remove(fn.c_str());
        return;
    }

    ofs.open(fn.c_str(), ios_base::binary);
    Date da;
    da.set_date();
    short a = da.get_short();
    ofs.write((char*)&a, 2);
    for (; wl_daily.count_total() != 0;)
    {
        w = wl_daily.pop();
        wl_daily.delete_node(0);
        ofs.write((char*)&w.id, 2);
        ofs.write((char*)&w.st, 1);
        ofs.write((char*)&w.ti, 1);
        ofs.write((char*)&w.da, 2);
    }
    ofs.close();
}

int Account_running::known_t()
{
    return wl_done.count_total();
}

void Account_running::create_daily_wordlist()
{
    int a = vid;
    string fn = string(to_string(a)) + string("1.dat");//ÿ�����񵥴�
    short* id = new short;
    char* st = new char;
    char* ti = new char;
    short* da = new short;
    ifstream ifs(fn.c_str(), ios_base::binary);
    char ch = ifs.get();
    bool ifs_ok = true;
    if (!ifs.is_open() || ifs.bad() || (ifs.eof()))
        ifs_ok = false;
    ifs.putback(ch);
    ifs.read((char*)da, 2);
    Date d(*da);
    if (d.istoday() && ifs_ok)
    {
        for (; ifs&&ifs_ok;)
        {
            char ch = ifs.get();
            if (!ifs.is_open() || ifs.bad() || (ifs.eof()))
            {
                ifs_ok = false;
                break;
            }
            ifs.putback(ch);
            ifs.read((char*)id, 2);
            ifs.read((char*)st, 1);
            ifs.read((char*)ti, 1);
            ifs.read((char*)da, 2);
            Date d(*da);
            Wordnode w(*id, *st, *ti, d);
            wl_daily.addword(w);
        }
    }
    else
    {
        for (; ifs&&ifs_ok;)
        {
            char ch = ifs.get();
            if (!ifs.is_open() || ifs.bad() || (ifs.eof()))
            {
                ifs_ok = false;
                break;
            }
            ifs.putback(ch);
            ifs.read((char*)id, 2);
            ifs.read((char*)st, 1);
            ifs.read((char*)ti, 1);
            ifs.read((char*)da, 2);

            //*da -= 1;//������

            Date d(*da);
            Wordnode w(*id, Wordnode::undone, *ti, d);//��һ����ȡĬ�����±�������st
            switch (*ti)
            {
            case Wordnode::none:wl_new.addword(w); break;
            case Wordnode::first:wv_undone[0]->push_back(w); break;
            case Wordnode::second:wv_undone[1]->push_back(w); break;
            case Wordnode::third:wv_undone[2]->push_back(w); break;
            case Wordnode::fourth:wv_undone[3]->push_back(w); break;
            case Wordnode::fifth:wl_done.addword(*id, *st, *ti, *da); break;
            }
        }
        for (int i = 3; i >= 0; i--)
        {
            int count = wv_undone[i]->size();
            for (int j = count - 1; j >= 0; j--)
            {
                Wordnode w = (*(wv_undone[i]))[j];
                if (w.isEbbinghaus())
                {
                    wl_daily.addword(w);
                    (*(wv_undone[i])).erase((*(wv_undone[i])).begin() + j);
                }
                if (wl_daily.count_total() == go) break;
            }
            if (wl_daily.count_total() == go) break;
        }
        if (wl_daily.count_total() != go)
        {
            for (; wl_daily.count_total() != go && wl_new.count_total() != 0;)
            {
                Wordnode w = (wl_new.access(0))->data;
                wl_daily.addword(w);
                wl_new.delete_node(0);
            }
        }
    }
    wl_daily.shuffle();
    ifs.close();
    delete id, st, ti, da;
};

int Account_running::learning_t()
{
    int count = 0;
    for (int i = 0; i < 4; i++)
    {
        count += wv_undone[i]->size();
    }
    return count;
}

int Account_running::word_goal()
{
    return go;
}

int Account_running::word_rest()
{
    if (wl_daily.count_total() == 0) return 0;
    return wl_daily.count_total() - wl_daily.count_done();
}

int Account_running::word_new()
{
    return wl_daily.count_new();
}

void Account_running::complete()
{
    Date da;
    da.get_date();
    date_list.operator+(da);
}

bool Account_running::iscomplete()
{
    Date da = date_list.pop().data;
    return da.istoday();
}

int Account_running::complete_t()
{
    return date_list.count();
}

void Account_running::add_new_word(short vid)
{
    Date da;
    da.set_date();
    Wordnode w(vid, Wordnode::undone, Wordnode::none, da);
    wl_new.addword(w);
}

//Bug
void Account_running::save()
{
    int b = vid;
    string file_name = string(to_string(b)) + string(".dat");
    ofstream ofs(file_name.c_str(), ios_base::binary);
    Wordnode w;
    for (int i = 0; i < wl_done.count_total(); i++)
    {
        w = wl_done.access(i)->data;
        ofs.write((char*)&w.id, 2);
        ofs.write((char*)&w.st, 1);
        ofs.write((char*)&w.ti, 1);
        ofs.write((char*)&w.da, 2);
    }
    for (int i = 0; i < wl_new.count_total(); i++)
    {
        w = wl_new.access(i)->data;
        ofs.write((char*)&w.id, 2);
        ofs.write((char*)&w.st, 1);
        ofs.write((char*)&w.ti, 1);
        ofs.write((char*)&w.da, 2);
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < (*(wv_undone[i])).size(); j++)
        {
            w = (*(wv_undone[i]))[j];
            ofs.write((char*)&w.id, 2);
            ofs.write((char*)&w.st, 1);
            ofs.write((char*)&w.ti, 1);
            ofs.write((char*)&w.da, 2);
        }
    }
    ofs.close();

    string fn = string(to_string(b)) + string("1.dat");//���ÿ������
    if (wl_daily.count_total() <= 0)
    {
        remove(fn.c_str());
        return;
    }

    ofs.open(fn.c_str(), ios_base::binary);
    Date da;
    da.set_date();
    short a = da.get_short();
    ofs.write((char*)&a, 2);
    for (int i = 0; i < wl_daily.count_total(); i++)
    {
        w = wl_daily.access(i)->data;
        ofs.write((char*)&w.id, 2);
        ofs.write((char*)&w.st, 1);
        ofs.write((char*)&w.ti, 1);
        ofs.write((char*)&w.da, 2);
    }
    ofs.close();
}

void AccountManageSystem::init()
{
    ifstream ifs("accountlist.dat", ios_base::binary);
    char ch = ifs.get();
    if (!ifs.is_open() || ifs.bad() || (ifs.eof()))
        return;
    ifs.putback(ch);
    char vid;
    char na[21];
    string md5;
    char tMd5[33];
    tMd5[32] = 0;
    short go = 25;// DEBUG
    while (!ifs.eof())
    {
        char ch = ifs.get();
        if (!ifs.is_open() || ifs.bad() || (ifs.eof()))
            return;
        ifs.putback(ch);
        ifs.read((char*)&vid, 1);
        ifs.read((char*)&na, ACCOUNT_NAME);
        ifs.read((char*)tMd5, 32);
        ifs.read((char*)&go, 2);
        md5 = string(tMd5);
        Account a(vid, string(na), md5);
        a.go = go;
        account_list.push_back(a);
    }
}

void AccountManageSystem::save()
{

    ofstream os("accountlist.dat", ios_base::binary | ios_base::trunc);
    if (account_list.size() != 0)
    {
        int count = account_list.size();
        for (char i = 0; i < count; i++)
        {
            os.write(&(account_list[i]).vid, 1);
            os.write((account_list[i]).na.c_str(), ACCOUNT_NAME);
            os.write((account_list[i]).md5.c_str(), 32);
            os.write((char*)&(account_list[i].go), 2);
        }
    }

    if (AccountManageSystem::ar != nullptr)
        AccountManageSystem::ar->save();



}

bool AccountManageSystem::sign_in(char vid, string password)
{
    if (vid > account_list.back().vid)return false;

    if (account_list[getindex(vid)].md5 == Account::encode_obj.Encode(password))
    {
        sign_out();
        ar = new Account_running(account_list[getindex(vid)]);// ��������д����ڴ����
        return true;
    }
    return false;
}

bool AccountManageSystem::new_account(string name, string password)
{
    for (int i = 0; i < account_list.size(); i++)
    {
        if (account_list[i].na == name)
        {
            std::cout << "new_account failed!" << std::endl;
            return false;
        }

    }
    string md5 = Account::encode_obj.Encode(password);

    int vid = 0;
    if (account_list.size() > 0)
    {
        vid = account_list.back().vid + 1;
    }
    else
    {
        vid = 0;
    }

    Account a(vid, name, md5);
    account_list.push_back(a);
    sign_in(vid, password);

    for (int i = 0; i < (sizeof(MyLevel4WordIndices) / sizeof(unsigned)); i++)
        AccountManageSystem::ar->add_new_word(MyLevel4WordIndices[i]);

    save();

    sign_out();

    return true;
}

bool AccountManageSystem::delete_account(char vid)
{
    string path = string(to_string(vid)) + string(".dat");
    remove(path.c_str());
    path = string(vid + "0") + string("1.dat");
    remove(path.c_str());
    int i = 0;
    for (; (account_list[i].vid != vid) || (i > account_list.size()); i++);
    if (i > account_list.size()) { return false; }
    account_list.erase(account_list.begin() + i);
    return true;
}

string AccountManageSystem::showname(char vid)
{
    return account_list[vid].na;
}

bool AccountManageSystem::sign_out()
{
    if (ar != nullptr)
    {
        delete ar;
        ar = nullptr;
        return true;
    }
    return false;
}

bool AccountManageSystem::sign_in(string account, std::string password)
{
    int account_vid = -1;
    sign_out();
    for (int i = 0; i < account_list.size(); i++)
    {
        if (account_list[i].na == account)
        {
            account_vid = account_list[i].vid;
            break;
        }
    }
    if (account_vid < 0)
        return false;
    else
        return sign_in((unsigned char)account_vid, password);
}


bool AccountManageSystem::delete_current_account()
{

    if (ar != nullptr)
    {
        int tvid = ar->vid;

        sign_out();

        for (int i = 0; i < AccountManageSystem::account_list.size(); i++)
        {
            if (tvid == account_list[i].vid)
            {
                AccountManageSystem::delete_account(i);
                AccountManageSystem::save();
                return true;
            }
        }
    }
    return false;
}


string AccountManageSystem::get_current_user_name()
{
    if (AccountManageSystem::ar != nullptr)
        return ar->na;
    else
        return std::string();
}


vector<std::string> AccountManageSystem::get_all_users_name()
{
    std::vector<std::string> result;
    for (auto n : account_list)
        result.push_back(n.na);
    return result;
}

bool AccountManageSystem::setpassword(string password)
{
    if (ar == nullptr)return false;
    Account* ac = &account_list[getindex(ar->vid)];
    ar->md5 = ac->md5 = Account::encode_obj.Encode(password);
    return true;
}

bool AccountManageSystem::setgoal(short go)
{
    if (ar == nullptr)return false;
    Account* ac = &account_list[getindex(ar->vid)];
    ar->go = ac->go = go;
    return true;
}

void AccountManageSystem::end()
{
    save();
    sign_out();
    account_list.clear();
}

char AccountManageSystem::getindex(char vid)
{
    for (int i = 0; i < account_list.size(); i++)
    {
        if (account_list[i].vid == vid)return i;
    }
}

Account_running* AccountManageSystem::ar = nullptr;

vector<Account> AccountManageSystem::account_list;

#endif // !ACCOUNT_CPP
