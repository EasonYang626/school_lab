from aip import AipOcr
import requests
import json
APP_ID = '00000000'
API_KEY = 'fad46ee1e2654ef5b857adefd17ecfb8'
SECRET_KEY = '78986563c59342898ce3041deaa90a99'
client = AipOcr(APP_ID, API_KEY, SECRET_KEY)


def get_file_content(filePath):
    with open(filePath, 'rb') as fp:
        return fp.read()


def image2text(fileName):
    image = get_file_content(fileName)
    dic_result = client.basicGeneral(image)
    res = dic_result['words_result']
    result = ''
    for m in res:
        result = result + str(m['words'])
    return result


def tuling_reply(url, apikey, msg):
    data = {  # 这个是在帮助手册上直接复制过来的，"url"=="https://www.kancloud.cn/turing/www-tuling123-com/718227"
        # """与reqType在同一级的参数有：{
        # reqType : 输入类型
        # perception : 输入信息
        # userInfo : 用户参数"""
        "reqType": 0,
        # '''# reqType为int类型，可以为空，
        # 输入类型:{
        # 0:文本(默认)
        # 1:图片
        # 2:音频
        # }'''
        "perception": {  # perception为用户输入信息，不允许为空
            # """
            # perception参数中的参数有：{
            # inputText : 文本信息
            # inputImage : 图片信息
            # inputMedia : 音频信息
            # selfInfo : 客户端属性
            # }
            # 注意：输入参数必须包含inputText或inputImage或inputMedia，可以是其中的任何一个\
            # 也可以是全部！
            # """
            # 用户输入文文信息
            "inputText": {  # inputText文本信息
                "text": msg
            },
            # 用户输入图片url
            "inputImage": {  # 图片信息，后跟参数信息为url地址，string类型
                "url": "https://cn.bing.com/images/"
            },
            # 用户输入音频地址信息
            "inputMedia": {  # 音频信息，后跟参数信息为url地址，string类型
                "url": "https://www.1ting.com/"
            },
            # 客户端属性信息
            "selfInfo": {  # location 为selfInfo的参数信息，
                "location": {  # 地理位置信息
                    "city": "深圳",  # 所在城市，不允许为空
                    "province": "广东省",  # 所在省份，允许为空
                    "street": "南山"  # 所在街道，允许为空
                }
            },
        },
        "userInfo": {  # userInfo用户参数，不允许为空
            # """
            # "userInfo包含的参数":{
            # "apiKey" : {
            #     "类型" : "String",
            #     "是否必须" : "Y ",
            #     "取值范围" : "32位",
            #     "说明" : "机器人标识"
            #     }
            # "userId" : {
            #     "类型" : "String",
            #     "是否必须" : "Y ",
            #     "取值范围" : "长度小于等于32位",
            #     "说明" : "用户唯一标识"
            #     }
            # "gropId" : {
            #     "类型" : "String",
            #     "是否必须" : "N ",
            #     "取值范围" : "长度小于等于64位",
            #     "说明" : "群聊唯一标识"
            #     }
            # "userIdName" : {
            #     "类型" : "String",
            #     "是否必须" : "N ",
            #     "取值范围" : "长度小于等于64位",
            #     "说明" : "群内用户昵称"
            #     }
            # }
            # """
            "apiKey": "d85ce8f152474b779188598b0cf0b511",  # 你注册的apikey,机器人标识,32位
            "userId": "anystring168"  # 随便填，用户的唯一标识，长度小于等于32位
        }
    }
    headers = {'content-type': 'application/json'}  # 必须是json
    r = requests.post(url, headers=headers, data=json.dumps(data))
    return r.json()


if __name__ == '__main__':
    apikey = 'd85ce8f152474b779188598b0cf0b511'  # 填入机器人的apikey
    url = 'http://openapi.tuling123.com/openapi/api/v2'  # 图灵机器人的v2.0接口地址
    # print(json.dumps(tuling_reply.data))
    while True:
        msg = input('(输入exit结束)> ').strip()
        if not msg:
            continue
        if msg == 'exit':
            break
        reply = tuling_reply(url, apikey, msg)
        print(reply["results"][0]["values"]["text"])  # 可以直接打印reply


# keyword = image2text('./test01.png')
# print(keyword)
# kv = {'wd':keyword}
# r = requests.get("http://www.baidu.com/s",params = kv)
# print(r.request.url)
# r.raise_for_status()
# print(len(r.text))
