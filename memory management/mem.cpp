#include <iostream>
#include <stack>

using namespace std;


void showResult(int pageMissNum, int pageFrontNum, stack<int> pageDesert) {
    cout << "缺页次数：" << pageMissNum << "次" << endl;
    cout << "一共调用：" << pageFrontNum << "次" << endl;
    cout << "缺页中断率：" << pageMissNum*1.0/pageFrontNum*100 << "%" << endl;
    cout << "淘汰页面：";

    stack<int> helpstack;   // 辅助栈，实现栈的逆置

    while (!pageDesert.empty()){
		helpstack.push(pageDesert.top());
        pageDesert.pop();
    }
    while (!helpstack.empty()){
        cout << helpstack.top() << " ";
        helpstack.pop();
    }
	cout << endl;
}
void FIFO(int pageFrameNum, int pageFrontNum, int* pages){

	stack<int> pageDesert; // 淘汰页面
    int pageFrame[pageFrameNum];    // 物理页框
    for(int i = 0 ; i < pageFrameNum ; i++){
        pageFrame[i] = 0;
    }

    int pageMissNum = 0;    // 缺页次数
    int count = 0;
    int helpNum = 0;    // 实现FIFO算法
    while (count < pageFrontNum)
    {
        cout << "第" << count+1 << "次：" << endl;
        
        bool isMiss = true;     // 判断本次是否缺页
        bool isEmpty = true;    // 判断物理页框是否有空位
        bool isExist = false;   // 判断物理页框中是否存在本次页面

        // 判断物理页框中是否已经存在本次页面
        for(int i = 0 ; i < pageFrameNum ; i++){
            if(pages[count] == pageFrame[i]){
                isExist = true;
                break;
            }
        }
        // 若本次页面在物理页框中已存在，则直接进入下次页面走向
        if(isExist){
            // cout << "本次页面走向页框中已经存在！" << endl;
            cout << "目前物理页框中页面编号为：";
            for(int i = 0 ; i < pageFrameNum ; i++){
                cout << pageFrame[i] << " ";
            }
            cout << endl;
            count++;
            continue;
        }
        // 判断物理页框有无空位
        for(int i = 0 ; i < pageFrameNum ; i++){
            if(pageFrame[i] == 0){
                isEmpty = true;
                break;
            }else{
                isEmpty = false;
            }
        }
        // 本次页面在物理页框中不存在，且有空位，按顺序放入
        if(!isExist && isEmpty){
            for (int i = 0 ; i < pageFrameNum; i++){
                if(pageFrame[i] == 0){
                    pageFrame[i] = pages[count];
                    break;
                }
            }
        }

        // 本次页面在物理页框中不存在，且物理页框中没有空位了
        // 实现 FIFO 算法
        if(!isExist && !isEmpty){
            pageDesert.push(pageFrame[helpNum%pageFrameNum]);   // 淘汰页面入栈
            pageFrame[helpNum%pageFrameNum] = pages[count];
            helpNum++;
        }

        // 计算缺页次数
        if(isMiss == true){
            pageMissNum++;
        }

        cout << "目前物理页框中页面编号为：";
        for (int i = 0 ; i < pageFrameNum ; i++){
            cout << pageFrame[i] << " ";
        }
        cout << endl;
        count++;
    }
    cout << endl;
    showResult(pageMissNum, pageFrontNum, pageDesert);
    return;
}

// set for LRU
struct PageFrame{
	int pageId;
	int useTime;
};
void LRU(int pageFrameNum, int pageFrontNum, int* pages) {
	stack<int> pageDesert; // 淘汰页面

	PageFrame pageFrame[pageFrameNum];	// 物理页框
    for(int i = 0 ; i < pageFrameNum ; i++){
        pageFrame[i].pageId = 0;
		pageFrame[i].useTime = pageFrontNum + 1;
    }

    int pageMissNum = 0;    // 缺页次数
    int count = 0;
    while (count < pageFrontNum)
    {
        cout << "第" << count+1 << "次：" << endl;
        
        bool isMiss = true;     // 判断本次是否缺页
        bool isEmpty = true;    // 判断物理页框是否有空位
        bool isExist = false;   // 判断物理页框中是否存在本次页面

        // 判断物理页框中是否已经存在本次页面
        for(int i = 0 ; i < pageFrameNum ; i++){
            if(pages[count] == pageFrame[i].pageId){
				pageFrame[i].useTime = count;
                isExist = true;
                break;
            }
        }
        // 若本次页面在物理页框中已存在，则直接进入下次页面走向
        if(isExist){
            // cout << "本次页面走向页框中已经存在！" << endl;
            cout << "目前物理页框中页面编号为：";
            for(int i = 0 ; i < pageFrameNum ; i++){
                cout << pageFrame[i].pageId << " ";
            }
            cout << endl;
            count++;
            continue;
        }
        // 判断物理页框有无空位
        for(int i = 0 ; i < pageFrameNum ; i++){
            if(pageFrame[i].pageId == 0){
                isEmpty = true;
                break;
            }else{
                isEmpty = false;
            }
        }
        // 本次页面在物理页框中不存在，且有空位，按顺序放入
        if(!isExist && isEmpty){
            for (int i = 0 ; i < pageFrameNum; i++){
                if(pageFrame[i].pageId == 0){
                    pageFrame[i].pageId = pages[count];
					pageFrame[i].useTime = count;
                    break;
                }
            }
        }
        // 本次页面在物理页框中不存在，且物理页框中没有空位了
        // 实现 LRU 算法
        if(!isExist && !isEmpty){
			int minTime = pageFrame[0].useTime;
			int changePageId = pageFrame[0].pageId;
			int index = 0;
            for (int i = 0 ; i < pageFrameNum ; i++){
				if (minTime > pageFrame[i].useTime){
					minTime = pageFrame[i].useTime;
					changePageId = pageFrame[i].pageId;
					index = i;
				}
            }
			pageDesert.push(pageFrame[index].pageId);  // 淘汰页面入栈
			pageFrame[index].pageId = pages[count];
			pageFrame[index].useTime = count;
        }
        // 计算缺页次数
        if(isMiss == true){
            pageMissNum++;
        }
        cout << "目前物理页框中页面编号为：";
        for (int i = 0 ; i < pageFrameNum ; i++){
            cout << pageFrame[i].pageId << " ";
        }
        cout << endl;
        count++;
    }
    cout << endl;
    showResult(pageMissNum, pageFrontNum, pageDesert);
    return;
}

void OPT(int pageFrameNum, int pageFrontNum, int* pages) {
	stack<int> pageDesert; // 淘汰页面
    int pageFrame[pageFrameNum];    // 物理页框
    for(int i = 0 ; i < pageFrameNum ; i++){
        pageFrame[i] = 0;
    }

    int pageMissNum = 0;    // 缺页次数
    int count = 0;
    while (count < pageFrontNum)
    {
        cout << "第" << count+1 << "次：" << endl;
        
        bool isMiss = true;     // 判断本次是否缺页
        bool isEmpty = true;    // 判断物理页框是否有空位
        bool isExist = false;   // 判断物理页框中是否存在本次页面

        // 判断物理页框中是否已经存在本次页面
        for(int i = 0 ; i < pageFrameNum ; i++){
            if(pages[count] == pageFrame[i]){
                isExist = true;
                break;
            }
        }
        // 若本次页面在物理页框中已存在，则直接进入下次页面
        if(isExist){
            // cout << "本次页面走向页框中已经存在！" << endl;
            cout << "目前物理页框中页面编号为：";
            for(int i = 0 ; i < pageFrameNum ; i++){
                cout << pageFrame[i] << " ";
            }
            cout << endl;
            count++;
            continue;
        }
        // 判断物理页框有无空位
        for(int i = 0 ; i < pageFrameNum ; i++){
            if(pageFrame[i] == 0){
                isEmpty = true;
                break;
            }else{
                isEmpty = false;
            }
        }
        // 本次页面在物理页框中不存在，且有空位，按顺序放入
        if(!isExist && isEmpty){
            for (int i = 0 ; i < pageFrameNum; i++){
                if(pageFrame[i] == 0){
                    pageFrame[i] = pages[count];
                    break;
                }
            }
        }
        // 本次页面在物理页框中不存在，且物理页框中没有空位了
        // 实现 OPT 算法
        if(!isExist && !isEmpty){
            bool isExistEle = false;    // 是否存在未来不再出现的元素
            bool isFound = false;       // 是否找到未来下标的元素
            int frameIndex = 0; // 记录的物理页框下标
            stack<int> helpStack0;   // 辅助栈
            for(int i = 0 ; i < pageFrameNum ; i++){
                for(int j = count ; j < pageFrontNum ; j++){
                    if(pageFrame[i] == pages[j]){ // 若当前物理页框中，不存在未来不再出现的元素
                        helpStack0.push(j); // 记录当前未来将遇见的下标
                        isFound = true;     // 找到未来下标的元素
                    }
                }
                // 当前物理页框中，存在未来不再出现的元素
                if(!isFound){
                    frameIndex = i; // 记录当前物理页框
                    isExistEle = true;  // 记录未来不再出现的元素
                    break;
                }
                isFound = false;
            }
            if(isExistEle){ // 存在未来不再出现的元素
                pageDesert.push(pageFrame[frameIndex]); // 淘汰页面入栈
                pageFrame[frameIndex] = pages[count];
            }else{ // 不存在未来不再出现的元素
                int t = 0;
                while(!helpStack0.empty()){ // 找出栈中最大元素
                    if(t < helpStack0.top()){
                        t = helpStack0.top();
                    }
                    helpStack0.pop();
                }
                for(int i = 0 ; i < pageFrameNum ; i++){
                    if(pageFrame[i] == pages[t]){
                        pageDesert.push(pageFrame[i]);
                        pageFrame[i] = pages[count];
                    }
                }
            }
        }

        // 计算缺页次数
        if(isMiss == true){
            pageMissNum++;
        }
        cout << "目前物理页框中页面编号为：";
        for (int i = 0 ; i < pageFrameNum ; i++){
            cout << pageFrame[i] << " ";
        }
        cout << endl;
        count++;
    }
    cout << endl;

    showResult(pageMissNum, pageFrontNum, pageDesert);
    return;
}

int main(){
	int pageFrameNum;    // 物理页框数
    
    int pageFrontNum;    // 页面走向个数


    cout << "请输入分配给该作业的物理页框数：";
    cin >> pageFrameNum;

    cout << "请输入要访问的逻辑页面个数：";
    cin >> pageFrontNum;

    int pages[pageFrontNum]; // 页面走向
    // c++中数组必须赋初值
    for(int i = 0 ; i < pageFrontNum ; i++){
        pages[i] = 0;
    }

	cout << "请输入运行过程中对逻辑页面的访问顺序：";
    for(int i = 0 ; i < pageFrontNum ; i++){
        cin >> pages[i];    // 获取页面走向数组
    }

	cout << "FIFO begins: " << endl;
	FIFO(pageFrameNum, pageFrontNum, pages);
	cout << "FIFO ends: " << endl << endl;

	cout << "LRU begins: " << endl;
	LRU(pageFrameNum, pageFrontNum, pages);
	cout << "LRU ends: " << endl << endl;
	
	cout << "OPT begins: " << endl;
	OPT(pageFrameNum, pageFrontNum, pages);
	cout << "OPT ends: " << endl << endl;
	return 0;
}