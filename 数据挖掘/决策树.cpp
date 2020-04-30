#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
//��������
typedef enum _outlook
{
    sunny,
    overcast,
    rain
} _outlook;
//�¶�����
typedef enum _temperature
{
    hot,
    mild,
    cool
} _temperature;
//ʪ������
typedef enum _humidity
{
    high,
    normal
} _humidity;
//��ǿ������
typedef enum _wind
{
    weak,
    strong
} _wind;
//��������(������Ŀ������)
typedef enum _attributeTypeName
{
    outlook,
    temperature,
    humidity,
    wind
} _attributeTypeName;
//Ŀ������
typedef enum _targetAttribute
{
    yes,
    no
} _targetAttribute;
//���Խṹ��(������Ŀ������)
typedef struct _attribute
{
    _outlook outlook;
    _temperature temperature;
    _humidity humidity;
    _wind wind;
} _attribute, *pAttribute;
//���������ṹ��,�������Ժ�Ŀ������
typedef struct _sample
{
    _attribute attributes;            //����
    _targetAttribute targatAttribute; //Ŀ������
} _sample, *pSample;
//ѵ�������ṹ��,�� ID3 �㷨��������ʾѵ��������
typedef struct _examples
{
    int sampleNum;                          //ѵ������������
    int attributeTypeNum;                   //ѵ����������������������Ŀ(������Ŀ������)
    _attributeTypeName *pAttributeTypeName; //ѵ������������������
    int *pTrainSample;                      //ѵ��������
} _examples, *pExamples;
//���������ṹ��
typedef struct _node
{
    _attributeTypeName attributeTypeName; //�ڵ������������������
    int childNum;                         //��֧��
    int *pChildAttributeValue;            //ÿ���ӽڵ��Ӧ������ֵ
    struct _node **pChildNode;            //ָ���ӽ��
    struct _node *pParent;                //ָ�򸸽��
    int lable;                            //Ҷ�ӽڵ������ֵ(Ŀ������)
} _node, *pNode;
//�����������Ͷ�Ӧ���ַ���(��д),������ӡ�������ľ�����
char *attributeTypeString[] =
    {
        "OUTLOOK",
        "TEMPERATURE",
        "HUMIDITY",
        "WIND",
};
//��������ֵ��Ӧ���ַ���(Сд),������ӡ�������ľ�����
char *attributeValueString[][3] =
    {
        {"sunny", "overcast", "rain"},
        {"hot", "mild", "cool"},
        {"high", "normal", "error"},
        {"weak", "strong", "error"},
};
//����Ŀ�����Զ�Ӧ���ַ���(Сд),������ӡ�������ľ�����
char *attributeTargeteString[] =
    {
        "yes",
        "no",
};
//����ÿ���������ͺ�����ֵ֮������ӷ�
char *connectString = "--->";
//���������洢�����������ÿ����������ַ�����
#define MAX_ID3_CHAR_LENGTH 500
//ѵ��ʵ��
#define TRAIN_NUM 14 //���� 14 ��ѵ��ʵ��
#define ATTR_NUM 4   //ÿ��ѵ��ʵ���� 4 ������
_sample trainSample[TRAIN_NUM] = {
    {{sunny, hot, high, weak}, no},          //1
    {{sunny, hot, high, strong}, no},        //2
    {{overcast, hot, high, weak}, yes},      //3
    {{rain, mild, high, weak}, yes},         //4
    {{rain, cool, normal, weak}, yes},       //5
    {{rain, cool, normal, strong}, no},      //6
    {{overcast, cool, normal, strong}, yes}, //7
    {{sunny, mild, high, weak}, no},         //8
    {{sunny, cool, normal, weak}, yes},      //9
    {{rain, mild, normal, weak}, yes},       //10
    {{sunny, mild, normal, strong}, yes},    //11
    {{overcast, mild, high, strong}, yes},   //12
    {{overcast, hot, normal, weak}, yes},    //13
    {{rain, mild, high, strong}, no},        //14
};
//����ѵ�������������������͸�������
//����ѵ��������
//���ѵ�������������������͸�������
void CalcPosAndNegNum(pExamples pNodeExamples, int *pPosNum, int *pNegNum)
{
    int i;
    //ȡ��ѵ������
    int *pTrainSample = pNodeExamples->pTrainSample;
    //���� 0
    *pPosNum = 0;
    *pNegNum = 0;
    for (i = 0; i < pNodeExamples->sampleNum; i++)
    {
        //pTrainSample Ϊ ѵ �� �� �� �� �� ַ �� ÿ �� ѵ �� �� �� �� ��
        //(pNodeExamples->attributeTypeNum+1(Ŀ������))������
        //����Ŀ�����Ե�ַ�洢�� yes �� no ֵ������ pPosNum �� pNegNum
        if (*(pTrainSample + i * ((pNodeExamples->attributeTypeNum) + 1) + pNodeExamples->attributeTypeNum) == yes)
        {
            (*pPosNum)++;
        }
        if (*(pTrainSample + i * ((pNodeExamples->attributeTypeNum) + 1) + pNodeExamples->attributeTypeNum) == no)
        {
            (*pNegNum)++;
        }
    }
    return;
}
//����ѵ������������
//�������������͸�������
//���ؼ��������ֵ
double CalcEntropy(int posNum, int negNum)
{
    double entropy;
    //�������������������һ��Ϊ 0,�򷵻���ֵΪ 0
    if (posNum == 0 || negNum == 0)
    {
        entropy = 0;
    }
    else
    {
        //�� int ��ת��Ϊ double ��
        double fPosNum = (double)posNum;
        double fNegNum = (double)negNum;
        //���������������ı���
        double pPos = fPosNum / (fPosNum + fNegNum);
        double pNeg = fNegNum / (fPosNum + fNegNum);
        //������ֵ
        entropy = -pPos * (log10(pPos) / log10((double)2)) - pNeg * (log10(pNeg) / log10((double)2));
    }
    return entropy;
}
//�ҳ���Ϣ������ߵ�����
//����ѵ�������ṹ��,���������͸�������
//���ؾ��������Ϣ�����������ѵ�������е�ƫ���Լ�������԰���������ֵ�����Լ�
//�����ǵ�ֵ
int FindMaxInfoGainAttrbute(pExamples pNodeExamples, int posNum, int negNum, int *pMaxInfoGainAttrValueNum, int *pMaxInfoGainAttrValue)
{
    int i, j, k;
    //����ѵ������������
    double entropyS = CalcEntropy(posNum, negNum);
    //�ڼ���������Ե���Ϣ����ʱ���ڴ洢���Ա�������
    double entropyA = 0, entropyV = 0;
    //����ÿ�����������¹��ж��������͵����Ա���
    int attributeVlaueTypeNum;
    //�洢ÿ�����������µ����Ա�����ֵ
    int *attributeValueName = (int *)malloc(pNodeExamples->sampleNum * 4);
    //�洢ÿ�����������¸����Ա�����ֵ�ĸ���
    int *attributeValueNum = (int *)malloc(pNodeExamples->sampleNum * 4);
    //�洢ÿ�����Ա�����Ŀ�����Ե���������
    int *attributeTargetValuePosNum = (int *)malloc(pNodeExamples->sampleNum * 4);
    //�洢ÿ�����Ա�����Ŀ�����Եĸ�������
    int *attributeTargetValueNegNum = (int *)malloc(pNodeExamples->sampleNum * 4);
    //��ʼ��ѵ�������ĵ�һ������Ϊ��Ϣ������������,����Ϣ����Ϊ 0
    double maxInfoGain = 0;
    int maxInfoGainOffset = 0;
    *pMaxInfoGainAttrValueNum = 1;
    *pMaxInfoGainAttrValue = *pNodeExamples->pTrainSample;
    //��������ڴ�ʧ��
    if (attributeValueName == NULL || attributeValueNum == NULL ||
        attributeTargetValuePosNum == NULL || attributeTargetValueNegNum == NULL)
    {
        *pMaxInfoGainAttrValueNum = -1;
        return -1;
    }
    //����ÿ�����Ե���Ϣ����,ͬʱȡ����Ϣ�����������Լ�ƫ��
    for (i = 0; i < pNodeExamples->attributeTypeNum; i++)
    {
        //���Ե���Ϣ����
        double infoGain;
        //ȡ����һ��ѵ�����������Ա���������ֵ�洢
        *attributeValueName = *(pNodeExamples->pTrainSample + i);
        //���Ա���������ʼ��Ϊ 1
        attributeVlaueTypeNum = 1;
        //��һ�����Ա����ĸ�����ʼ��Ϊ 1
        *attributeValueNum = 1;
        //�жϵ�һ��ѵ��������Ŀ�����Ե�����
        if (*(pNodeExamples->pTrainSample + pNodeExamples->attributeTypeNum) == yes)
        {
            //����������ʼ��Ϊ 1
            *attributeTargetValuePosNum = 1;
            //����������ʼ��Ϊ 0
            *attributeTargetValueNegNum = 0;
        }
        else
        {
            //����������ʼ��Ϊ 0
            *attributeTargetValuePosNum = 0;
            //����������ʼ��Ϊ 1
            *attributeTargetValueNegNum = 1;
        }
        //���±� 1 ��ʼͳ�Ƹ������Ա��������
        for (j = 1; j < pNodeExamples->sampleNum; j++)
        {
            //�����ж�ȡ��������ֵ�Ƿ�Ϊ�³��ֵ�����ֵ
            int compareFlag = 0;
            //ȡ�����Ա���������ֵ
            int attributeValue = *(pNodeExamples->pTrainSample +
                                   j * (pNodeExamples->attributeTypeNum + 1) + i);
            //��ȡ��������ֵ��֮ǰ�洢������ֵ���Ƚ�,���Ƿ�Ϊ�µ�����ֵ
            for (k = 0; k < attributeVlaueTypeNum; k++)
            {
                //����Աȳɹ�
                if (attributeValue == *(attributeValueName + k))
                {
                    //��Ӧ���Ա��������� 1
                    *(attributeValueNum + k) = *(attributeValueNum + k) + 1;
                    //�жϸ�ѵ��������Ŀ�����Ե�����
                    if (*(pNodeExamples->pTrainSample + pNodeExamples->attributeTypeNum + j * (pNodeExamples->attributeTypeNum + 1)) == yes)
                    {
                        //���������� 1
                        *(attributeTargetValuePosNum + k) = *(attributeTargetValuePosNum + k) + 1;
                    }
                    else
                    {
                        //���������� 1
                        *(attributeTargetValueNegNum + k) = *(attributeTargetValueNegNum + k) + 1;
                    }
                    //�Աȳɹ���־�� 1
                    compareFlag = 1;
                    break;
                }
            } //end for(k=0;k<attributeVlaueTypeNum;k++)
            //���û�жԱȳɹ�,˵���³���������ֵ
            if (compareFlag == 0)
            {
                //�洢�³��ֵ����Ա���������ֵ
                *(attributeValueName + attributeVlaueTypeNum) = attributeValue;
                //�³��ֵ����Ա���������ʼ��Ϊ 1
                *(attributeValueNum + attributeVlaueTypeNum) = 1;
                //�ж��³��ֵ�����ֵ��������Ŀ�����Ե�����
                if (*(pNodeExamples->pTrainSample + pNodeExamples->attributeTypeNum + j * (pNodeExamples->attributeTypeNum + 1)) == yes)
                {
                    //����������ʼ��Ϊ 1
                    *(attributeTargetValuePosNum + attributeVlaueTypeNum) = 1;
                    //����������ʼ��Ϊ 0
                    *(attributeTargetValueNegNum + attributeVlaueTypeNum) = 0;
                }
                else
                {
                    //����������ʼ��Ϊ 0
                    *(attributeTargetValuePosNum + attributeVlaueTypeNum) = 0;
                    //����������ʼ��Ϊ 1
                    *(attributeTargetValueNegNum + attributeVlaueTypeNum) = 1;
                }
                //���Ա��������� 1
                attributeVlaueTypeNum++;
            } //end if(compareFlag==0)
        }     //end for(j=1;j<*(pNodeExamples->pTrainSample);j++)
        //�������Է���ѵ���������ص�����ֵ
        entropyA = 0; //�� 0
        for (j = 0; j < attributeVlaueTypeNum; j++)
        {
            //����� j �����Ա�������
            entropyV = CalcEntropy(*(attributeTargetValuePosNum + j), *(attributeTargetValueNegNum + j));
            entropyA = entropyA + (double)(*(attributeValueNum + j)) / (double)(pNodeExamples->sampleNum) * entropyV;
        }
        //�������Ե���Ϣ����
        infoGain = entropyS - entropyA;
        //Ѱ����Ϣ������������
        if (maxInfoGain < infoGain)
        {
            //���������Ϣ�������Ϣ
            maxInfoGain = infoGain;
            maxInfoGainOffset = i;
            *pMaxInfoGainAttrValueNum = attributeVlaueTypeNum;
            for (j = 0; j < attributeVlaueTypeNum; j++)
            {
                *(pMaxInfoGainAttrValue + j) = *(attributeValueName + j);
            }
        }
    } //end for(i=0;i<pNodeExamples->attributeTypeNum;i++)
    //�ͷ��ڴ�ռ�
    free(attributeValueName);
    free(attributeValueNum);
    free(attributeTargetValuePosNum);
    free(attributeTargetValueNegNum);
    return maxInfoGainOffset;
}
//ID3 �����㷨
//����ѵ��������,����ѵ��������������������Ŀ���������ͺ�ѵ������,�Լ����ø��㷨
//�ĸ��ڵ�
//���ؼ�����ĸ��ڵ�
pNode ID3(pExamples pNodeExamples)
{
    int i, j, k;
    //����һ��ָ��������ʱ�����ڴ�
    int *pPointTemp;
    //�洢��Ϣ������ߵ����Ե���������ֵ
    int *pMaxInfoGainAttrValue;
    //�洢��Ϣ������ߵ�������ѵ�������е�����
    int maxInfoGainOffset;
    //�洢��Ϣ������ߵ����԰���������ֵ����(��֧��)
    int maxInfoGainAttrValueNum;
    //����һ�����������
    pNode pTreeNode;
    //�洢ѵ������Ŀ������Ϊ yes �� no ��������
    int posNum, negNum;
    //���ѵ��������������С�� 1��˵�������������ֱ�ӷ��� NULL
    if (pNodeExamples->sampleNum < 1)
    {
        return NULL;
    }
    //����ѵ�������������������͸�������
    CalcPosAndNegNum(pNodeExamples, &posNum, &negNum);
    //������������͸���������Ϊ 0�������
    if (posNum == 0 && negNum == 0)
    {
        return NULL;
    }
    //Ϊ�������ڵ�����ڴ�ռ�
    pTreeNode = (pNode)malloc(sizeof(_node));
    //�������ʧ�� �����ش���
    if (pTreeNode == NULL)
    {
        return NULL;
    }
    //��ʶ�ýڵ�Ϊ��Ҷ�ӽڵ�
    pTreeNode->lable = 0xff;
    //�����������Ϊ 0����ʶ�ýڵ�Ŀ������ֵΪ no ��ʾ�ýڵ�ΪҶ�ӽڵ�
    if (posNum == 0)
    {
        pTreeNode->lable = no;
        return pTreeNode;
    }
    //�����������Ϊ 0����ʶ�ýڵ�Ŀ������ֵΪ yes ��ʾ�ýڵ�ΪҶ�ӽڵ�
    if (negNum == 0)
    {
        pTreeNode->lable = yes;
        return pTreeNode;
    }
    //���������ĿС�� 1����ʶ�ýڵ�Ҷ�ӽڵ㣬��Ŀ������ֵΪ yes �� no �бȽ϶��һ
    //��
    //���������Ŀһ���࣬��Ŀ������ֵΪ no
    if (pNodeExamples->attributeTypeNum < 1)
    {
        if (posNum > negNum)
        {
            pTreeNode->lable = yes;
        }
        else
        {
            pTreeNode->lable = no;
        }
        return pTreeNode;
    }
    //�ȸ��洢����������Ե�����ֵ��ָ������ѵ��������������һ����С����ʱ��
    //��
    pPointTemp = (int *)malloc(pNodeExamples->sampleNum * 4);
    if (pPointTemp == NULL)
    {
        free(pTreeNode);
        return NULL;
    }
    //�ҳ���Ϣ������ߵ�����
    maxInfoGainOffset = FindMaxInfoGainAttrbute(pNodeExamples, posNum, negNum, &maxInfoGainAttrValueNum, pPointTemp);
    //���������Ϣ�������,�򷵻ش���
    if (maxInfoGainOffset == -1)
    {
        //�ͷŽڵ�ռ�õ��ڴ�ռ�
        free(pPointTemp);
        free(pTreeNode);
        return NULL;
    }
    //���洢����������Ե�����ֵ��ָ�����ͳ�Ƴ��� maxInfoGainAttrValueNum ��С��
    //��ռ�
    pMaxInfoGainAttrValue = (int *)malloc(maxInfoGainAttrValueNum * 4);
    if (pMaxInfoGainAttrValue == NULL)
    {
        free(pPointTemp);
        free(pTreeNode);
        return NULL;
    }
    //������������Ե�����ֵ�洢�� pMaxInfoGainAttrValue ��
    for (i = 0; i < maxInfoGainAttrValueNum; i++)
    {
        *(pMaxInfoGainAttrValue + i) = *(pPointTemp + i);
    }
    //�ͷ���ʱ���� pPointTemp ռ�õ��ڴ�ռ�
    free(pPointTemp);
    //����Ϣ������ߵ��������ƴ洢���ڵ���
    pTreeNode->attributeTypeName = *(pNodeExamples->pAttributeTypeName + maxInfoGainOffset);
    //�洢�ڵ�ķ�֧������Ϣ������ߵ����Ե�����ֵ������
    pTreeNode->childNum = maxInfoGainAttrValueNum;
    //�洢ÿ���ӽڵ��Ӧ������ֵ
    pTreeNode->pChildAttributeValue = pMaxInfoGainAttrValue;
    //�ȷ��� maxInfoGainAttrValueNum ��ָ�븳���ڵ���ӽڵ�
    pTreeNode->pChildNode = (struct _node **)malloc(sizeof(struct _node **) * maxInfoGainAttrValueNum);
    if (pTreeNode->pChildNode == NULL)
    {
        free(pTreeNode);
        return NULL;
    }
    //����ӽڵ�ָ���ָ�����
    for (i = 0; i < maxInfoGainAttrValueNum; i++)
    {
        *(pTreeNode->pChildNode + i) = NULL;
    }
    //���´��������Ϣ������ߵ����������µ�ÿ������ֵ\
//��ȡ����ÿ������ֵ���Ӧ��ѵ����������,�ݹ����� ID3 �㷨
    //������ѵ��������һ����С����ʱ�ռ����洢��ÿ������ֵ��ȡ������������
    pPointTemp = (int *)malloc(pNodeExamples->sampleNum * (pNodeExamples->attributeTypeNum + 1) * 4);
    //��ÿ������ֵ������ȡ��������
    for (i = 0; i < maxInfoGainAttrValueNum; i++)
    {
        //ȡ���� i ������ֵ
        int attrValue = *(pMaxInfoGainAttrValue + i);
        //ͳ������ѵ������������������Ϣ�������������� i ������ֵ��ͬ��������
        int attrValueNum = 0;
        //������ѵ������������ȡ����
        for (j = 0; j < pNodeExamples->sampleNum; j++)
        {
            //�жϵ� j ��ѵ�������ĵ� maxInfoGainOffset ������ֵ�Ƿ�� attrValue ���
            if (attrValue == *(pNodeExamples->pTrainSample + j * (pNodeExamples->attributeTypeNum + 1) +
                               maxInfoGainOffset))
            {
                //��ƥ���ѵ�������洢����ʱ�ڴ� pPointTemp ��
                for (k = 0; k < (pNodeExamples->attributeTypeNum + 1); k++)
                {
                    *(pPointTemp + attrValueNum * (pNodeExamples->attributeTypeNum + 1) + k) =
                        *(pNodeExamples->pTrainSample + j * (pNodeExamples->attributeTypeNum + 1) + k);
                }
                //ƥ���������� 1
                attrValueNum++;
            }
        }
        //�������� i �����Ե�������Ϊ��
        if (attrValueNum == 0)
        {
            //����һ���ӽڵ�
            pNode pTreeNodeI = (pNode)malloc(sizeof(_node));
            if (pTreeNodeI == NULL)
            {
                free(pPointTemp);
                free(pTreeNode);
                return NULL;
            }
            //��ʶ�ýڵ�Ҷ�ӽڵ㣬��Ŀ������ֵΪ yes �� no �бȽ϶��һ��
            if (posNum > negNum)
            {
                pTreeNodeI->lable = yes;
            }
            else
            {
                pTreeNodeI->lable = no;
            }
            //�Ѹýڵ㸳�����ڵ�ĵ� i ���ӽڵ�
            *(pTreeNode->pChildNode + i) = pTreeNodeI;
        }    //end if(attrValueNum==0)
        else //if(attrValueNum==0)
        {
            //��ѵ�����������Ƴ��� i ������ֵ,������ϳ��µ�ѵ��������
            int l;
            //����һ���ڵ��ѵ�������ṹ�� examples
            pExamples pNodeExamplesTemp;
            //Ϊ�µ�ѵ�������������ڴ�ռ�
            int *pTrainSample = (int *)malloc(pNodeExamples->attributeTypeNum * attrValueNum * 4);
            if (pTrainSample == NULL)
            {
                free(pPointTemp);
                free(pTreeNode);
                return NULL;
            }
            //Ϊѵ�������ṹ�� examples �����ڴ�ռ�
            pNodeExamplesTemp = (pExamples)malloc(sizeof(_examples));
            if (pNodeExamplesTemp == NULL)
            {
                free(pPointTemp);
                free(pTreeNode);
                return NULL;
            }
            //������ת�Ƶ��µ�ѵ�������� pTrainSample ��
            l = 0;
            for (j = 0; j < attrValueNum; j++)
            {
                for (k = 0; k < (pNodeExamples->attributeTypeNum + 1); k++)
                {
                    // �ѳ�ȥ�����Ϣ���������֮�������ѵ�������洢��
                    //pTrainSample ��
                    if (k != maxInfoGainOffset)
                    {
                        *(pTrainSample + l) = *(pPointTemp + j * (pNodeExamples->attributeTypeNum + 1) + k);
                        l++;
                    }
                }
            }
            //�洢ѵ������
            pNodeExamplesTemp->pTrainSample = pTrainSample;
            //Ϊ pNodeExampls ���������ͷ���ռ�
            k = (pNodeExamples->attributeTypeNum - 1) * 4;
            pNodeExamplesTemp->pAttributeTypeName = (enum _attributeTypeName *)malloc((pNodeExamples->attributeTypeNum - 1) * 4);
            if (pNodeExamplesTemp->pAttributeTypeName == NULL)
            {
                free(pPointTemp);
                free(pTreeNode);
                return NULL;
            }
            //�洢���˵� maxInfoGainOffset ֮���������������
            k = 0;
            for (j = 0; j < (pNodeExamples->attributeTypeNum); j++)
            {
                if (j != maxInfoGainOffset)
                {
                    *(pNodeExamplesTemp->pAttributeTypeName + k) = *(pNodeExamples->pAttributeTypeName + j);
                    k++;
                }
            }
            //�洢������������
            pNodeExamplesTemp->attributeTypeNum = pNodeExamples->attributeTypeNum - 1;
            //�洢ѵ������������
            pNodeExamplesTemp->sampleNum = attrValueNum;
            //�� pNodeExamplesTemp Ϊѵ��������,�� pTreeNode Ϊ���ڵ�ݹ���� ID3
            //�㷨
            *(pTreeNode->pChildNode + i) = ID3(pNodeExamplesTemp);
            if (*(pTreeNode->pChildNode + i) == NULL)
            {
                free(pPointTemp);
                free(pTreeNode);
                return NULL;
            }
            //�ͷŸ� pNodeExamplesTemp ������ڴ�
            free(pNodeExamplesTemp->pAttributeTypeName);
            free(pNodeExamplesTemp->pTrainSample);
            free(pNodeExamplesTemp);
        } //end else //if(attrValueNum==0)
    }     //end for(i=0;i<maxInfoGainAttrValueNum;i++)
    //�ͷŴ洢��ÿ������ֵ��ȡ�����������ݵ���ʱ�ռ�
    free(pPointTemp);
    //�������ɵĽڵ�
    return pTreeNode;
}
//������������Թ��򼯺ϵ���ʽ��ӡ����
//������������ڵ��Ҫ��ӡ���ַ���
//�����
void DisPlayID3Result(pNode pRootNode, char *printString)
{
    int i;
    //����һ���µ��ַ����������洢���ýڵ�֮ǰ�Ĵ�ӡ��Ϣ
    char prePrintString[MAX_ID3_CHAR_LENGTH];
    //���жϸýڵ��Ƿ�ΪҶ�ӽڵ�
    if (pRootNode->lable != 0xff)
    {
        //�����Ҷ�ӽڵ�,���Ҷ�ӽڵ������ֵ(Ŀ������)���ӵ� printString �в���ӡ��
        //��
        //strcat(printString,connectString);
        strcat(printString, attributeTargeteString[pRootNode->lable]);
        printf("%s\n", printString);
        return;
    }
    //�ѷ�Ҷ�ӽڵ�������������ӵ� printString ��
    strcat(printString, attributeTypeString[pRootNode->attributeTypeName]);
    //�����������͵�����ֵ�����ӷ�
    strcat(printString, connectString);
    //���ݸýڵ���ӽڵ�������ÿ���ӽڵ�����ֵ���ӵ� prePrintString ��
    for (i = 0; i < pRootNode->childNum; i++)
    {
        //�� printString ����ַ������Ƶ� prePrintString ��
        strcpy(prePrintString, printString);
        //��ÿ���ӽڵ������ֵ���ӵ� prePrintString ��
        strcat(prePrintString, attributeValueString[pRootNode->attributeTypeName][*(pRootNode->pChildAttributeValue + i)]);
        //��������ֵ����һ����������(��Ŀ������)�����ӷ��� prePrintString ��
        strcat(prePrintString, connectString);
        //�ݹ����� DisPlayID3Result ����ӡÿһ���ӽڵ���������ͺ�����ֵ
        DisPlayID3Result(*(pRootNode->pChildNode + i), prePrintString);
    }
    return;
}
//�ͷž�������ÿ���ڵ�ռ�õ��ڴ�ռ�
//������������ڵ�
//�����
void freeNode(pNode pRootNode)
{
    int i;
    //�����Ҷ�ӽڵ�,���ͷŸýڵ�ռ�õ��ڴ�ռ�
    if (pRootNode->lable != 0xff)
    {
        free(pRootNode);
        return;
    }
    //�������Ҷ�ӽڵ�
    for (i = 0; i < pRootNode->childNum; i++)
    {
        //�ȵݹ���� freeNode �ͷ�ÿ���ӽڵ�ռ�õ��ڴ�ռ�
        freeNode(*(pRootNode->pChildNode + i));
    }
    //�ͷ�ָ��÷�Ҷ�ӽڵ��ÿ���ӽڵ������ֵռ�õ��ڴ�ռ�;
    free(pRootNode->pChildAttributeValue);
    //�ͷű��ڵ�ռ�õ��ڴ�ռ�
    free(pRootNode);
    return;
}
//�������㷨����
void DecisionTree(void)
{
    //����������ĸ��ڵ�
    pNode pRootNode = NULL;
    //������ڵ��ѵ������������������
    _attributeTypeName pRootAttributeName[4] = {outlook, temperature, humidity, wind};
    //��ԭʼѵ��ʵ����ʼ�����ڵ��ѵ�������ṹ�� examples
    pExamples pRootExampls = (pExamples)malloc(sizeof(_examples));
    if (pRootExampls == NULL)
    {
        printf("���ڵ�ѵ�������ṹ���ڴ����ʧ��!");
        return;
    }
    //�洢���ڵ��ѵ������������������
    pRootExampls->pAttributeTypeName = pRootAttributeName;
    //������ڵ��ѵ��������������
    pRootExampls->sampleNum = sizeof(trainSample) / sizeof(_sample);
    //�洢���ڵ��ѵ����������������������
    pRootExampls->attributeTypeNum = ATTR_NUM;
    //�����ڵ��ѵ������
    pRootExampls->pTrainSample = (int *)trainSample;
    //ID3 �㷨
    pRootNode = ID3(pRootExampls);
    //��ӡ��������Ϣ
    if (pRootNode)
    {
        //������������Թ��򼯺ϵ���ʽ��ӡ����
        char ID3_Result[MAX_ID3_CHAR_LENGTH] = "Rule:";
        DisPlayID3Result(pRootNode, ID3_Result);
    }
    else
    {
        printf("ID3 ����������ʧ��!");
    }
    //�ڴ��ͷ�
    free(pRootExampls);
    if (pRootNode)
    {
        freeNode(pRootNode);
    }
    return;
}
int main(void)
{
    //���þ������㷨
    DecisionTree();
    return 1;
}