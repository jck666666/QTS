#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;
ifstream input_file;

void read() {
	input_file.open("20210501-20210531.csv", ios::in);//讀入的檔名
	string line;
	while (getline(input_file, line)) {
		string tmp;
		stringstream ss(line);//字串處理
		while (getline(ss, tmp, ','))//csv檔是逗號分隔檔，所以透過逗號來讀取檔案裡面的資料
		{
			cout << tmp << endl;
		}

	}
	input_file.close();//開檔要記得關檔
}

int main()
{
	read();
	//system("pause");
	return 0;

}
