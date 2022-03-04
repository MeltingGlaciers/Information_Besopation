#include <iostream>
#include <string>
#include <bitset>
#include <fstream>
#include <vector>
#define uint unsigned int
#define ull unsigned long long
#define uchar unsigned char
using namespace std;

#define TO_BIN(a) \
	bitset<sizeof(a)*CHAR_BIT>(a)


ull StringToULL(uchar* str) {

	ull bin = 0;
	for (int i = 0; i < 8; i++) {

		bin = (bin << 8) | str[i];
	}
	return bin;
}

uchar* ULLToString(ull bin) {

	uchar* str = new uchar[8];

	for (int i = 0; i < 8; i++) {

		str[i] = ((bin << (8 * i)) >> 56);
	}

	return str;
}

uint CyrcleR32(uint a, int n) {

	return (a >> n) | (a << (32 - n));

}

ull CyrcleR64(ull a, int n) {

	return (a >> n) | (a << (64 - n));

}

uint CyrcleL32(uint a, int n) {

	return (a << n) | (a >> (32 - n));

}

uint CryptFunction(uint r, uint key) {

	return CyrcleL32(r, 9) ^ (~(CyrcleR32(key, 11) ^ r));
	//return r;

}

uint KeyModifier(ull key, int i) {

	ull tempKey = CyrcleR64(key, i * 4);
	tempKey = tempKey << 27;
	tempKey = tempKey >> 32;
	return tempKey;

}

ull FeistelBlockEncryption(ull block, ull key, int roundCount) {

	uint left = block >> 32;
	uint right = block;
	uint temp;
	for (int i = 0; i < roundCount; i++) {

		uint currKey = KeyModifier(key, i);
		left = left ^ CryptFunction(right, currKey);
		temp = left;
		left = right;
		right = temp;

	}

	ull newRight = left;
	ull newLeft = right;
	newLeft = newLeft << 32;


	return newRight + newLeft;
}

ull FeistelBlockDecryption(ull block, ull key, int roundCount) {

	uint left = block >> 32;
	uint right = block;

	uint temp;
	for (int i = roundCount - 1; i >= 0; i--) {

		uint currKey = KeyModifier(key, i);
		left = left ^ CryptFunction(right, currKey);

		temp = left;
		left = right;
		right = temp;


	}



	ull newRight = left;
	ull newLeft = right;
	newLeft = newLeft << 32;


	return newRight + newLeft;
}

uchar* FeistelEncryption(uchar*message, int *mem, ull key, int roundCount, ull vector) {

	int needToAdd = (*mem-1) % 8;
	int oldMem = *mem;
	*mem = *mem - 1 + (8 - needToAdd);
	uchar* newMessage = new uchar[*mem];

	for (int i = 0; i < oldMem; i++) {

		newMessage[i] = message[i];
		

	}

	for (int i = oldMem; i < *mem; i++) {

		newMessage[i] = 0;

	}

	/*for (int i = 0; i < needToAdd; i++) {

		newMessage[i] = 0;


	}

	for (int i = needToAdd; i < *mem; i++) {
	
		newMessage[i] = message[i];

	}*/


	int blockCount = *mem / 8;

	uchar** blocks = new uchar * [blockCount];

	for (int i = 0; i < blockCount; i ++) {

		blocks[i] = new uchar[8];
		
		for (int j = 0; j < 8; j++) {

			blocks[i][j] = newMessage[j + i * 8];
		}

	}

	ull* crypted = new ull[blockCount];
	
	for (int i = 0; i < blockCount; i++) {

		ull bin = StringToULL(blocks[i]);

		if (vector != NULL) {

			if (i == 0) bin = bin ^ vector;
			else bin = bin ^ crypted[i - 1];

		}


		crypted[i] = FeistelBlockEncryption(bin, key, roundCount);

	}

	uchar* cryptMessage = new uchar[*mem];

	for (int i = 0; i < blockCount; i++) {

		blocks[i] = ULLToString(crypted[i]);

	}


	for (int i = 0; i < blockCount; i++) {


		for (int j = 0; j < 8; j++) {

			cryptMessage[j + i * 8] = blocks[i][j];

		}

		cout << "Блок " << i << endl << TO_BIN(crypted[i]) << endl;

	}

	return cryptMessage;

}

uchar* FeistelDecryption(uchar message[], int* mem, ull key, int roundCount, ull vector) {

	int blockCount = *mem / 8;

	uchar** blocks = new uchar * [blockCount];

	for (int i = 0; i < blockCount; i++) {

		blocks[i] = new uchar[8];

		for (int j = 0; j < 8; j++) {

			blocks[i][j] = message[j + i * 8];

		}

	}
	
	ull* decrypted = new ull[blockCount];

	for (int i = 0; i < blockCount; i++) {

		ull bin = StringToULL(blocks[i]);

		decrypted[i] = FeistelBlockDecryption(bin, key, roundCount);

		if (vector != NULL) {

			if (i == 0) decrypted[i] = decrypted[i] ^ vector;
			else decrypted[i] = decrypted[i] ^ StringToULL(blocks[i-1]);

		}

	}

	uchar* decryptMessage = new uchar[*mem];

	for (int i = 0; i < blockCount; i++) {

		blocks[i] = ULLToString(decrypted[i]);

	}

	for (int i = 0; i < blockCount; i++) {


		for (int j = 0; j < 8; j++) {

			decryptMessage[j + i * 8] = blocks[i][j];

		}
		cout << "Блок " << i<<endl << TO_BIN(decrypted[i]) << endl;

	}

	return decryptMessage;

}




int main()
{
	setlocale(LC_ALL, "Russian");

	string message;
	ifstream file("text.txt");
	getline(file, message);
	vector<uchar> sV(message.begin(), message.end());
	sV.push_back('\0');
	uchar* s = &sV[0];
	ull key = 68719476736 + 32;
	int mem = message.length();

	cout << "Изначальное сообщение \n" << s << endl<<endl;

	uchar* en = FeistelEncryption(s,&mem, key,2, 12344);

	cout << "-------------------------------------------------------------------------------------------------" << endl;
	cout << "Закодированное сообщение \n";
	for (int i = 0; i < mem; i++) {

		cout << en[i];

	}
	cout << endl;

	uchar* de = FeistelDecryption(en,&mem, key, 2,12344);

	cout << "Расшифрованное сообщение \n" << de << endl << endl;
	

	
	
	

	

}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
//  0000 0000 0000 0000 0000 0000 0000 0001 / 0000 0000 0011 0000 0000 0010 0000 0000

//
