// MakeCM.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "FixSMC.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/uuid/sha1.hpp>

using namespace std;


int main()
{
	CFixSMC fsmc;
	st_fix fix;

	ifstream ifs("E:\\new_crackme\\crackme\\Release\\info.log");
	string strItem;
	vector<string> vItem;
	
	
	if (ifs.is_open())
	{	
		while (!ifs.eof())
		{
			ifs >> strItem;
			vItem.push_back(strItem);
		} 
		ifs.close();
	}
	cout <<"���������� "<<vItem.size() << " ���ʼ����..."<<endl;
	fix.st_oid.dwOID = strtoul(vItem[0].c_str(), 0, 16);
	fix.st_oid.oid.dwStart = strtoul(vItem[1].c_str(), 0, 16);
	fix.st_oid.oid.dwEnd = strtoul(vItem[2].c_str(), 0, 16);
	fix.st_oid.dwDDS = strtoul(vItem[3].c_str(), 0, 16);
	//cout << "������ CMainDlg::OnInitialDlg ����ʼƫ��(HEX):" << endl;
	//cin >>hex>> fix.st_oid.dwOID;
	//cout << "������ CMainDlg::OnInitialDlg �ڲ�������ֹλ��,�ո�ָ�(HEX):" << endl;
	//cin >> hex >> fix.st_oid.oid.dwStart >> fix.st_oid.oid.dwEnd;
	//cout << "������ CMainDlg::OnInitialDlg �ڲ���ȡ��������ݶ���ƫ��(HEX):" << endl;
	//cin >> hex >> fix.st_oid.dwDDS;

	fix.st_sg.dwSG = strtoul(vItem[4].c_str(), 0, 16);
	fix.st_sg.smc_in.dwStart = strtoul(vItem[5].c_str(), 0, 16);
	fix.st_sg.smc_in.dwEnd = strtoul(vItem[6].c_str(), 0, 16);
	fix.st_sg.dwTM = strtoul(vItem[7].c_str(), 0, 16);
	//cout << "������ CMainDlg::StartGame ����ʼƫ��(HEX):" << endl;
	//cin >> hex >> fix.st_sg.dwSG;
	//cout << "������ CMainDlg::StartGame �ڲ�SMC������ֹλ��,�ո�ָ�(HEX):" << endl;
	//cin >> hex >> fix.st_sg.smc_in.dwStart >> fix.st_sg.smc_in.dwEnd;
	//cout << "������ CMainDlg::OnTimer ����ʼƫ��(HEX):" << endl;
	//cin >> hex >> fix.st_sg.dwTM;

	fix.dwLG = strtoul(vItem[8].c_str(), 0, 16);
	//cout << "������ CMaze::LetsGo ����ʼƫ��(HEX):" << endl;
	//cin >> hex >> fix.dwLG;

	//// CMaze::LetsGo
	//fix.dwLG = 0x22210;
	fix.dwMME = strtoul(vItem[9].c_str(), 0, 16);
	//cout << "������ CMatrixMul::MMulEqual ����ʼƫ��(HEX):" << endl;
	//cin >> hex >> fix.dwMME;

	//// CMatrixMul::MMulEqual
	//fix.dwMME = 0x1FF50;
	fix.st_mvt.dwMvt = strtoul(vItem[10].c_str(), 0, 16);
	fix.st_mvt.crc.dwStart = strtoul(vItem[11].c_str(), 0, 16);
	fix.st_mvt.crc.dwEnd = strtoul(vItem[12].c_str(), 0, 16);
	fix.st_mvt.mvt2.dwStart = strtoul(vItem[13].c_str(), 0, 16);
	fix.st_mvt.mvt2.dwEnd = strtoul(vItem[14].c_str(), 0, 16);
	fix.st_mvt.dwDDS = strtoul(vItem[15].c_str(), 0, 16);

	//cout << "������ CMainDlg::MoveThere ����ʼƫ��(HEX):" << endl;
	//cin >> hex >> fix.st_mvt.dwMvt;

	//cout << "������ CMainDlg::MoveThere �ڲ�CRC32У�����ֹƫ��(HEX):" << endl;
	//cin >> hex >> fix.st_mvt.crc.dwStart >> fix.st_mvt.crc.dwEnd;

	//cout << "������ CMainDlg::MoveThere �ڲ�SMC���ܵ���ֹƫ��,�ո�ָ�(HEX):" << endl;
	//cin >> hex >> fix.st_mvt.mvt2.dwStart >> fix.st_mvt.mvt2.dwEnd;

	//cout << "������ CMainDlg::MoveThere ����ȡ�������ݶ�ƫ��(HEX):" << endl;
	//cin >> hex >> fix.st_mvt.dwDDS;

	fix.st_init.dwInit  = strtoul(vItem[16].c_str(), 0, 16);
	fix.st_init.Init.dwStart = strtoul(vItem[17].c_str(), 0, 16);
	fix.st_init.Init.dwEnd = strtoul(vItem[18].c_str(), 0, 16);
	//cout << "������ CMainDlg::Init ��ʼƫ��(HEX):" << endl;
	//cin >> hex >> fix.st_init.dwInit;

	//cout<< "������ CMainDlg::Init �ڲ�SMC������ֹƫ��,�ո�ָ�(HEX):" << endl;
	//cin >> hex >> fix.st_init.Init.dwStart >> fix.st_init.Init.dwEnd;

	fix.st_DecLetsGo.dwStart = strtoul(vItem[19].c_str(), 0, 16);
	fix.st_DecLetsGo.dwEnd = strtoul(vItem[20].c_str(), 0, 16);
	fix.st_DecMMul.dwStart = strtoul(vItem[21].c_str(), 0, 16);
	fix.st_DecMMul.dwEnd = strtoul(vItem[22].c_str(), 0, 16);
	fix.st_EncLetsGo.dwStart = strtoul(vItem[23].c_str(), 0, 16);
	fix.st_EncLetsGo.dwEnd = strtoul(vItem[24].c_str(), 0, 16);
	fix.st_EncMMul.dwStart = strtoul(vItem[25].c_str(), 0, 16); 
	fix.st_EncMMul.dwEnd = strtoul(vItem[26].c_str(), 0, 16);
	fix.st_OnIdle.dwStart = strtoul(vItem[27].c_str(), 0, 16);
	fix.st_OnIdle.dwEnd = strtoul(vItem[28].c_str(), 0, 16);

	//cout << "������ CMainDlg::DecLetsGoWithCrcOfOID �ڲ�SMC������ֹƫ��,�ո�ָ�(HEX):" << endl;
	//cin >> hex >> fix.st_DecLetsGo.dwStart>> fix.st_DecLetsGo.dwEnd;
	//
	//cout << "������ CMainDlg::DecMMulEqualWithCrcOfInit �ڲ�SMC������ֹƫ��,�ո�ָ�(HEX):" << endl;
	//cin >> hex >> fix.st_DecMMul.dwStart >> fix.st_DecMMul.dwEnd;

	//cout << "������CMainDlg::EncLetsGoWithCrcOfOID �ڲ�SMC������ֹƫ��,�ո�ָ�(HEX):" << endl;
	//cin >> hex >> fix.st_EncLetsGo.dwStart >> fix.st_EncLetsGo.dwEnd;

	//cout << "������ CMainDlg::EncMMulEqualWithCrcOfInit �ڲ�SMC������ֹƫ��,�ո�ָ�(HEX):" << endl;
	//cin >> hex >> fix.st_EncMMul.dwStart >> fix.st_EncMMul.dwEnd;

	//cout << "������ CMainDlg::OnIdle �ڲ�SMC������ֹƫ�ƣ��ո�ָ�(HEX):" << endl;
	//cin >> hex >> fix.st_OnIdle.dwStart >> fix.st_OnIdle.dwEnd;
	
	fix.st_DecStartGame.lbDecStartGame.dwStart = strtoul(vItem[29].c_str(), 0, 16);
	fix.st_DecStartGame.lbDecStartGame.dwEnd = strtoul(vItem[30].c_str(), 0, 16);
	fix.st_DecStartGame.dwDecStartGame = strtoul(vItem[31].c_str(), 0, 16);

	fix.st_EncStartGame.lbEncStartGame.dwStart = strtoul(vItem[32].c_str(), 0, 16);
	fix.st_EncStartGame.lbEncStartGame.dwEnd = strtoul(vItem[33].c_str(), 0, 16);
	fix.st_EncStartGame.dwEncStartGame = strtoul(vItem[34].c_str(), 0, 16);

	//InitGame
	fix.st_InitGame.dwInitGame = strtoul(vItem[35].c_str(), 0, 16);
	fix.st_InitGame.InitGame.dwStart = strtoul(vItem[36].c_str(), 0, 16);
	fix.st_InitGame.InitGame.dwEnd = strtoul(vItem[37].c_str(), 0, 16);

	//Play
	fix.st_Play.dwPlay = strtoul(vItem[38].c_str(), 0, 16);
	fix.st_Play.Play.dwStart = strtoul(vItem[39].c_str(), 0, 16);
	fix.st_Play.Play.dwEnd = strtoul(vItem[40].c_str(), 0, 16);

	//Walk
	fix.st_Walk.dwWalk = strtoul(vItem[41].c_str(), 0, 16);
	fix.st_Walk.Walk.dwStart = strtoul(vItem[42].c_str(), 0, 16);
	fix.st_Walk.Walk.dwEnd = strtoul(vItem[43].c_str(), 0, 16);

	//IsGetThrough
	fix.st_IsGetThrough.dwIsGetThrough = strtoul(vItem[44].c_str(), 0, 16);
	fix.st_IsGetThrough.IsGetThrough.dwStart = strtoul(vItem[45].c_str(), 0, 16);
	fix.st_IsGetThrough.IsGetThrough.dwEnd = strtoul(vItem[46].c_str(), 0, 16);

	fix.st_HandleWinEvent.dwStart = strtoul(vItem[47].c_str(), 0, 16);
	fix.st_HandleWinEvent.dwSize = strtoul(vItem[48].c_str(), 0, 16);

	fsmc.Fix(fix);
	cout << "���������� " << fsmc.GetFixCount() << " ��������޸�!" << endl;
	system("pause");
    return 0;
}

