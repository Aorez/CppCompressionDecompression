#include"pch.h"
#include"functions.h"
#include<vector>
#include<string>
#include<set>
#include<map>
#include<iostream>
//using std::pair;

/*
�ո�ǰ����ַ�
�п��������֣���ĸ������
1 =
= 1
t e
e =
= e
�»��߹�����ĸ����
*/

//static std::vector<std::string> beforeText0;//Դ�ļ��ı�
//static std::vector<std::string> compressionText;//ѹ���ո��ע�ͺ���ı�
//static std::string compressionText2;//����ѹ���ı���һ�У�
//static int nowOpen = 0;
//static std::string decompressionTextBefore;//��ѹ֮ǰ���ı�
//static std::vector<std::string> decompressionTextAfter;//��ѹ����ı�
//static std::vector<int> toBinary;

std::vector<std::string> beforeText0;//Դ�ļ��ı�
std::vector<std::string> compressionText;//ѹ���ո��ע�ͺ���ı�
std::string compressionText2;//����ѹ���ı���һ�У�
int nowOpen = 0;
std::string decompressionTextBefore;//��ѹ֮ǰ���ı�
std::vector<std::string> decompressionTextAfter;//��ѹ����ı�
std::vector<int> toBinary;

//, ;  ()  {  }  #   ^ ? : .[] + -*/ % = > < !~| &
//&&  ||  ==  >=  <=  !=  ++  --  ::  <<  >>   +=  -=  *=  /=  %=  &=  ^=  ->
//������-1��ʾ���ո���-2��ʾ

std::set<std::string> numbers = { "0", "1", "2", "3", "4", "5",\
"6", "7", "8", "9" };

std::set<std::string> lower = { "_", "\\", \
"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z" };
std::set<std::string> upper = { \
"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z" };

std::set<std::string> unaryOperator = { ",", ";", "(", ")", "{", "}", "#",\
"^", "?", ":", ".", "[", "]", "+", "-", "*", "/", "%", "=", ">", "<", "!",\
"~", "|", "&", "\"", "'" };

std::set<std::string> operator0 = { ",", ";", "(", ")", "{", "}", "#",\
"?", ".", "[", "]", "~", "\"", "'" };

std::set<std::string> binaryOperator = { "&&", "||", "++", ">=", "<=", "!=",\
"++", "--", "::", "<<", ">>", "+=", "-=", "*=", "/=", "%=", "&=",\
"^=", "->", "==" };

std::set<std::string> words = { "asm", "auto", "begin", "bool", "break", "case", "catch", "char", "cin", "class", "const", "const_cast", "continue", "cout", "default", "define", "delete", "do", "double", "dynamic_cast", "else", "end", "endif", "endl", "enum", "erase", "except", "explicit", "export", "extern", "false", "finally", "float", "for", "friend", "goto", "if", "ifdef", "ifndef", "include", "inline", "insert", "int", "iostream", "iterator", "long", "main", "map", "mutable", "namespace", "new", "operator", "pragma", "private", "protected", "public", "register", "return", "set", "short", "signed", "size", "sizeof", "static", "static_cast", "std", "string", "struct", "switch", "template", "this", "throw", "true", "try", "typedef", "typename", "union", "unsigned", "using", "vector", "virtual", "void", "volatile", "while", };

std::map<std::string, int> ttoc;//text to code
std::map<int, std::string> ctot;//code to text

void dictInit() {
	int n = 2;
	for (auto i : lower) {
		ttoc.insert(std::pair<std::string, int>(i, n));
		ctot.insert(std::pair<int, std::string>(n, i));
		n++;
	}
	for (auto i : upper) {
		ttoc.insert(std::pair<std::string, int>(i, n));
		ctot.insert(std::pair<int, std::string>(n, i));
		n++;
	}
	for (auto i : numbers) {
		ttoc.insert(std::pair<std::string, int>(i, n));
		ctot.insert(std::pair<int, std::string>(n, i));
		n++;
	}
	for (auto i : unaryOperator) {
		ttoc.insert(std::pair<std::string, int>(i, n));
		ctot.insert(std::pair<int, std::string>(n, i));
		n++;
	}
	for (auto i : binaryOperator) {
		ttoc.insert(std::pair<std::string, int>(i, n));
		ctot.insert(std::pair<int, std::string>(n, i));
		n++;
	}
	for (auto i : words) {
		ttoc.insert(std::pair<std::string, int>(i, n));
		ctot.insert(std::pair<int, std::string>(n, i));
		n++;
	}
}

void compression2() {
	compressionText2 = "";
	toBinary.clear();
	std::vector<std::string> compressionText1 = compressionText;
	for (int i = 0; i < compressionText1.size(); i++) {
		for (int j = 0; j < compressionText1[i].size(); j++) {
			char nowChar = compressionText1[i][j];
			std::string nowWord = "";
			if (nowChar == ' ') {
				compressionText2 += std::string(1, '1') + " ";
				toBinary.push_back(1);
			}
			//��������֣����ֵ�������
			else if (nowChar > 47 && nowChar <= 57 || nowChar == '\\') {
				int x = ttoc[std::string(1, nowChar)];
				compressionText2 += std::to_string(x) + " ";
				toBinary.push_back(x);
			}
			//�������ĸ���»���
			else if (nowChar >= 65 && nowChar <= 90 || nowChar >= 97 && nowChar <= 122 || nowChar == '_') {
				nowWord += nowChar;
				while (true) {
					if (j + 1 < compressionText1[i].size()) {
						nowChar = compressionText1[i][j + 1];
						//�ҵ���������
						if (nowChar >= 65 && nowChar <= 90 || nowChar >= 97 && nowChar <= 122 || nowChar == '_') {
							nowWord += nowChar;
							j++;
							continue;
						}
						else {
							//���ǹؼ���
							if (words.find(nowWord) == words.end()) {
								for (char c : nowWord) {
									int x = ttoc[std::string(1, c)];
									compressionText2 += std::to_string(x) + " ";
									toBinary.push_back(x);
								}
							}
							//�ǹؼ���
							else {
								int x = ttoc[nowWord];
								compressionText2 += std::to_string(x) + " ";
								toBinary.push_back(x);
							}
							break;
						}
					}
					else {
						//���ǹؼ���
						if (words.find(nowWord) == words.end()) {
							for (char c : nowWord) {
								int x = ttoc[std::string(1, c)];
								compressionText2 += std::to_string(x) + " ";
								toBinary.push_back(x);
							}

						}
						//�ǹؼ���
						else {
							int x = ttoc[nowWord];
							compressionText2 += std::to_string(x) + " ";
							toBinary.push_back(x);
						}
						break;
					}
				}
			}
			//����Ƕ�Ԫ������ĵ�һ��
			else if (operator0.find(std::string(1, nowChar)) == operator0.end()) {
				if (j + 1 < compressionText1[i].size()) {
					//std::string nowStr = "" + nowChar + compressionText1[i][j + 1];
					char nowStr[2] = { nowChar, compressionText1[i][j + 1] };
					//std::cout << nowStr << std::endl;
					//����ö�Ԫ���������
					if (binaryOperator.find(nowStr) != binaryOperator.end()) {
						int x = ttoc[nowStr];
						compressionText2 += std::to_string(x) + " ";
						toBinary.push_back(x);
						j++;
					}
					//�ö�Ԫ����������ڣ�˵����һԪ�����
					else {
						int x = ttoc[std::string(1, nowChar)];
						compressionText2 += std::to_string(x) + " ";
						toBinary.push_back(x);
					}
				}
				else {
					int x = ttoc[std::string(1, nowChar)];
					compressionText2 += std::to_string(x) + " ";
					toBinary.push_back(x);
				}
			}
			//�����һԪ������ļ�����
			else if (unaryOperator.find(std::string(1, nowChar)) != unaryOperator.end()) {
				int x = ttoc[std::string(1, nowChar)];
				compressionText2 += std::to_string(x) + " ";
				toBinary.push_back(x);
			}
		}
		compressionText2 += "0 ";
		toBinary.push_back(0);
	}
	//compressionText2 += "0";
	toBinary.pop_back();
	compressionText2.pop_back();
	compressionText2.pop_back();
	//std::cout << compressionText2 << std::endl;
}

void compression() {
	//beforeText����ԭ�ļ���ÿ������
	//compressionText�����һ��ѹ������ļ���ÿ������
	compressionText.clear();
	std::vector<std::string> beforeText = beforeText0;
	for (int i = 0; i < beforeText.size(); i++) {
	loopi:	std::string beforeStr = beforeText[i];
		std::string afterStr = "";
		if (beforeStr.size() <= 0) {
			continue;
		}
		else if (beforeStr.size() == 1) {
			if (beforeStr == "{" || beforeStr == "}") {
				afterStr = beforeStr;
				std::string lastLine = compressionText.back();
				lastLine += afterStr;
				compressionText.pop_back();
				compressionText.push_back(lastLine);
				continue;
			}
		}
		//���е�����beforeStr����������1
		else {
			//����ÿ���ַ�
			for (int j = 0; j < beforeStr.size(); j++) {
				if (beforeStr[j] == '\t') {
					continue;
				}
				//�����ʱ���ַ�Ϊ'/'
				//�п�����ע��
				else if (beforeStr[j] == '/') {
					if (j + 1 < beforeStr.size()) {
						//������һ���ַ�������Ϊ'/'
						if (beforeStr[j + 1] == '/') {
							break;
						}
						//��һ���ַ�������Ϊ'*'
						else if (beforeStr[j + 1] == '*') {
							int k;
							//����������һ�е�ʣ������
							for (k = j + 2; k + 1 < beforeStr.size(); k++) {
								//�����һ�д���"*/"��˵��ע������һ�н���
								if (beforeStr[k] == '*' && beforeStr[k + 1] == '/') {
									j = k + 1;
									break;
								}
							}
							//���if����˵��ע���ں�������вŽ���
							if (k + 1 == beforeStr.size() || j + 2 == beforeStr.size()) {
								//�ȱ�����һ�е�����
								if (afterStr.size() > 0) {
									compressionText.push_back(afterStr);
								}
								//�������������е����ݣ��ҵ�"*/"
								for (int ik = i + 1; ik < beforeText.size(); ik++) {
									for (int jk = 0; jk + 1 < beforeText[ik].size(); jk++) {
										//����һ��"*/"�Լ�ǰ������ݶ�ȥ����������������·ŵ�beforeText��Ӧ�±���
										if (beforeText[ik][jk] == '*' && beforeText[ik][jk + 1] == '/') {
											std::string beforeStr2 = "";
											for (int kk = jk + 2; kk < beforeText[ik].size(); kk++) {
												beforeStr2 += beforeText[ik][kk];
											}
											beforeText[ik] = beforeStr2;
											i = ik;
											//���������ѭ��
											goto loopi;
										}
									}
								}
							}
						}
						else {
							afterStr += beforeStr[j];
						}
					}
					else {
						afterStr += beforeStr[j];
					}
				}
				//ѹ���ո�
				else if (beforeStr[j] == ' ') {
					if (j == 0) {
						continue;
					}
					else if (beforeStr[j - 1] == ' ') {
						continue;
					}
					else if (j + 1 == beforeStr.size()) {
						continue;
					}
					//���ǵ�һ���ַ���Ҳ�������һ���ַ���ͬʱǰ�治�ǿո�
					else {
						char nowChar = beforeStr[j - 1];
						//���nowCharǰ������Щ�ַ�
						if (nowChar >= 65 && nowChar <= 90 || nowChar >= 97 && nowChar <= 122 || nowChar > 47 && nowChar <= 57 || nowChar == '_') {
							char nowChar2 = beforeStr[j + 1];
							//���nowChar��������Щ�ַ�
							if (nowChar2 >= 65 && nowChar2 <= 90 || nowChar2 >= 97 && nowChar2 <= 122 || nowChar2 > 47 && nowChar2 <= 57 || nowChar2 == '_') {
								afterStr += ' ';
							}
						}
						else if (beforeStr[j - 1] == '"' && beforeStr[j + 1] == '"' || beforeStr[j - 1] == '\'' && beforeStr[j + 1] == '\'') {
							afterStr += ' ';
						}
					}
				}
				else {
					afterStr += beforeStr[j];
				}
			}
			if (afterStr.size() == 1) {
				if (afterStr == "{" || afterStr == "}") {
					std::string lastLine = compressionText.back();
					lastLine += afterStr;
					compressionText.pop_back();
					compressionText.push_back(lastLine);
				}
			}
			else if (afterStr.size() > 0) {
				compressionText.push_back(afterStr);
			}
		}

	}
}

void decompression() {
	//�൱��split(" ")
	std::vector<std::string> decompressionCodes;
	std::string codestring = "";

	//std::cout << decompressionTextBefore << std::endl;

	for (int i = 0; i < decompressionTextBefore.size(); i++) {
		if (decompressionTextBefore[i] == ' ') {
			if (codestring.size() > 0) {
				decompressionCodes.push_back(codestring);
				codestring = "";
			}
		}
		else {
			codestring += decompressionTextBefore[i];
		}
	}
	//for (int i = 0; i < decompressionCodes.size(); i++) {
	//	std::cout << decompressionCodes[i] << " ";
	//}
	//std::cout << std::endl;
	decompressionTextAfter.clear();
	std::string textstring = "";
	//��Ϊ����0�ű�־��һ�У�����Ҫ����
	decompressionCodes.push_back("0");
	for (int i = 0; i < decompressionCodes.size(); i++) {
		//�ո�
		if (decompressionCodes[i] == "1") {
			textstring += " ";
		}
		else if (decompressionCodes[i] == "0") {
			decompressionTextAfter.push_back(textstring);
			textstring = "";
		}
		else if (ctot.find(atoi(decompressionCodes[i].c_str())) == ctot.end()) {
			textstring += decompressionCodes[i];
		}
		else {
			textstring += ctot[atoi(decompressionCodes[i].c_str())];
		}
	}
}