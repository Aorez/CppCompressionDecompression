#include"pch.h"
#include"functions.h"
#include<vector>
#include<string>
#include<set>
#include<map>
#include<iostream>
//using std::pair;

/*
空格前后的字符
有可能是数字，字母，符号
1 =
= 1
t e
e =
= e
下划线归在字母里面
*/

//static std::vector<std::string> beforeText0;//源文件文本
//static std::vector<std::string> compressionText;//压缩空格和注释后的文本
//static std::string compressionText2;//编码压缩文本（一行）
//static int nowOpen = 0;
//static std::string decompressionTextBefore;//解压之前的文本
//static std::vector<std::string> decompressionTextAfter;//解压后的文本
//static std::vector<int> toBinary;

std::vector<std::string> beforeText0;//源文件文本
std::vector<std::string> compressionText;//压缩空格和注释后的文本
std::string compressionText2;//编码压缩文本（一行）
int nowOpen = 0;
std::string decompressionTextBefore;//解压之前的文本
std::vector<std::string> decompressionTextAfter;//解压后的文本
std::vector<int> toBinary;

//, ;  ()  {  }  #   ^ ? : .[] + -*/ % = > < !~| &
//&&  ||  ==  >=  <=  !=  ++  --  ::  <<  >>   +=  -=  *=  /=  %=  &=  ^=  ->
//换行用-1表示，空格用-2表示

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
			//如果是数字，数字单独编码
			else if (nowChar > 47 && nowChar <= 57 || nowChar == '\\') {
				int x = ttoc[std::string(1, nowChar)];
				compressionText2 += std::to_string(x) + " ";
				toBinary.push_back(x);
			}
			//如果是字母，下划线
			else if (nowChar >= 65 && nowChar <= 90 || nowChar >= 97 && nowChar <= 122 || nowChar == '_') {
				nowWord += nowChar;
				while (true) {
					if (j + 1 < compressionText1[i].size()) {
						nowChar = compressionText1[i][j + 1];
						//找到完整单词
						if (nowChar >= 65 && nowChar <= 90 || nowChar >= 97 && nowChar <= 122 || nowChar == '_') {
							nowWord += nowChar;
							j++;
							continue;
						}
						else {
							//不是关键字
							if (words.find(nowWord) == words.end()) {
								for (char c : nowWord) {
									int x = ttoc[std::string(1, c)];
									compressionText2 += std::to_string(x) + " ";
									toBinary.push_back(x);
								}
							}
							//是关键字
							else {
								int x = ttoc[nowWord];
								compressionText2 += std::to_string(x) + " ";
								toBinary.push_back(x);
							}
							break;
						}
					}
					else {
						//不是关键字
						if (words.find(nowWord) == words.end()) {
							for (char c : nowWord) {
								int x = ttoc[std::string(1, c)];
								compressionText2 += std::to_string(x) + " ";
								toBinary.push_back(x);
							}

						}
						//是关键字
						else {
							int x = ttoc[nowWord];
							compressionText2 += std::to_string(x) + " ";
							toBinary.push_back(x);
						}
						break;
					}
				}
			}
			//如果是二元运算符的第一个
			else if (operator0.find(std::string(1, nowChar)) == operator0.end()) {
				if (j + 1 < compressionText1[i].size()) {
					//std::string nowStr = "" + nowChar + compressionText1[i][j + 1];
					char nowStr[2] = { nowChar, compressionText1[i][j + 1] };
					//std::cout << nowStr << std::endl;
					//如果该二元运算符存在
					if (binaryOperator.find(nowStr) != binaryOperator.end()) {
						int x = ttoc[nowStr];
						compressionText2 += std::to_string(x) + " ";
						toBinary.push_back(x);
						j++;
					}
					//该二元运算符不存在，说明是一元运算符
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
			//如果在一元运算符的集合里
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
	//beforeText保存原文件的每行内容
	//compressionText保存第一次压缩后的文件的每行内容
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
		//该行的内容beforeStr的字数大于1
		else {
			//遍历每个字符
			for (int j = 0; j < beforeStr.size(); j++) {
				if (beforeStr[j] == '\t') {
					continue;
				}
				//如果此时的字符为'/'
				//有可能是注释
				else if (beforeStr[j] == '/') {
					if (j + 1 < beforeStr.size()) {
						//并且下一个字符存在且为'/'
						if (beforeStr[j + 1] == '/') {
							break;
						}
						//下一个字符存在且为'*'
						else if (beforeStr[j + 1] == '*') {
							int k;
							//继续遍历这一行的剩余内容
							for (k = j + 2; k + 1 < beforeStr.size(); k++) {
								//如果这一行存在"*/"，说明注释在这一行结束
								if (beforeStr[k] == '*' && beforeStr[k + 1] == '/') {
									j = k + 1;
									break;
								}
							}
							//这个if条件说明注释在后面的行中才结束
							if (k + 1 == beforeStr.size() || j + 2 == beforeStr.size()) {
								//先保存这一行的内容
								if (afterStr.size() > 0) {
									compressionText.push_back(afterStr);
								}
								//继续遍历后面行的内容，找到"*/"
								for (int ik = i + 1; ik < beforeText.size(); ik++) {
									for (int jk = 0; jk + 1 < beforeText[ik].size(); jk++) {
										//把这一行"*/"以及前面的内容都去掉，后面的内容重新放到beforeText对应下标中
										if (beforeText[ik][jk] == '*' && beforeText[ik][jk + 1] == '/') {
											std::string beforeStr2 = "";
											for (int kk = jk + 2; kk < beforeText[ik].size(); kk++) {
												beforeStr2 += beforeText[ik][kk];
											}
											beforeText[ik] = beforeStr2;
											i = ik;
											//继续最外层循环
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
				//压缩空格
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
					//不是第一个字符，也不是最后一个字符，同时前面不是空格
					else {
						char nowChar = beforeStr[j - 1];
						//如果nowChar前面是这些字符
						if (nowChar >= 65 && nowChar <= 90 || nowChar >= 97 && nowChar <= 122 || nowChar > 47 && nowChar <= 57 || nowChar == '_') {
							char nowChar2 = beforeStr[j + 1];
							//如果nowChar后面是这些字符
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
	//相当于split(" ")
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
	//因为遇到0才标志着一行，所以要补零
	decompressionCodes.push_back("0");
	for (int i = 0; i < decompressionCodes.size(); i++) {
		//空格
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