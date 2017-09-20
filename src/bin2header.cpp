#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

#include <cstdio>

#define len(a) (sizeof(a)/sizeof(*a))

string GetFileName(string f) {
#if defined (__WIN32__)
    int split = f.find_last_of('\\');
#else
    int split = f.find_last_of('/');
#endif
    f.erase(0, split + 1);
    return f;
}

int PrintUsage(string exe="bin2header") {
    cout << "\nbin2header version 0.0.1\n2011 Jordan Irwin <antumdeluge@gmail.com>\n\n\tUsage:\t" << exe << " file\n" << endl;
    return 1;
}

int main(int argc, char** argv) {
    string executable = argv[0];
    executable = GetFileName(executable);
    //int split = executable.find_last_of('/');
    //executable.erase(0, split + 1);

    //string usage = "\n\tUsage:\t" + executable + " infile\n";

    if (argc < 2) {
        //cout << usage << endl;
        //return 1;
        return PrintUsage(executable);
    }

    FILE* test;
    test = fopen(argv[1], "r");
    if (!test) {
        cout << "\nFile: \"" << argv[1] << "\" does not exist\n";
        //cout << usage << endl;
        //return 1;
        return PrintUsage(executable);
    }
    fclose(test);

    /* Get Filenames and Path */
    string path = argv[1];
    string filename = GetFileName(path);
    string hname = filename;

    /* START Remove Unwanted Characters */
    int hname_len = hname.length();
    char badchars[6] = {'\\', '+', '-', '*', ' '};
    int current = 0;
    int x = 0;
    for (current; current < hname_len; current++) {
        x = 0;
        for (x; x < len(badchars); x++) {
            if ((hname[current] == badchars[x]) or (hname[current] == '.'))
                hname.replace(current, 1, "_");
            if (filename[current] == badchars[x])
                filename.replace(current, 1, "_");
        }
    }
    /* END Remove Unwanted Characters */

    /* START Uppercase Name for Header */
    filename += ".h";

    char hname_upper[hname_len + 2];
    current = 0;
    for (current; current < len(hname_upper); current++) {
        hname_upper[current] = hname[current];
        hname_upper[current] = toupper(hname_upper[current]);
    }

    string name_upper_h = hname_upper;
    name_upper_h.append("_H");
    /* END Uppercase Name for Header */

    /* START Read Data In */
    ifstream infile;
    infile.open(path.c_str(), ifstream::binary);

    int data_length;
    infile.seekg(0, ifstream::end);
    data_length = infile.tellg();
    infile.seekg(0, ifstream::beg);

    char data[data_length];

    infile.read(data, data_length);
    infile.close();
    /* END Read Data In */

    /* START Read Data Out to Header */
    ofstream outfile(filename.c_str());
    outfile << "#ifndef " << name_upper_h.c_str() << "\n#define " << name_upper_h.c_str() << "\n\nstatic const unsigned char " << hname << "[] = {\n";

    current = 0;
    for (current; current < data_length; current++) {
        if ((current % 12) == 0) outfile << "    ";

        stringstream ss;
        ss << "0x" << hex << setw(2) << setfill('0') << (int) (unsigned char) data[current];
        outfile << ss.str();

        if ((current + 1) < data_length) outfile << ", ";

        if ((current % 12) == 11) outfile << "\n";
    }

    outfile << "\n};\n\n#endif\n";
    outfile.close();
    /* END Read Data Out to Header */

    return 0;
}
