// Programming01.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <stdio.h>
// Exercise 02
int main()
{
	char repeat = 'y';
	int grade, sum = 0, counter = 0;

	while (repeat != 'n')
	{
		if (repeat == 'y')
		{
			printf("%s", "Input grade: ");
			scanf("%d", &grade);
			sum += grade;
			counter++;
		}
		else
		{
			printf("%s\n", "invalid input");
		}

		printf("%s", "Input another grade? (y / n): ");
		scanf(" %c", &repeat);
	}


	int average = (sum / counter);

	if (average >= 90)
	{
		printf("The average is %d (A)", average);
	}
	else if (average >= 80)
	{
		printf("The average is %d (B)", average);
	}
	else if (average >= 70)
	{
		printf("The average is %d (C)", average);
	}
	else if (average >= 60)
	{
		printf("The average is %d (D)", average);
	}
	else
	{
		printf("The average is %d (F)", average);
	}

	getchar();
	getchar();

	return 0;
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
