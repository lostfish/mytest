Title: 建站备忘
Date: 2015-01-24
Category: Tech 
Tags: web, github, pelican
Author: lostfish
Disqus_identifier: jianzhanbeiwang
 
一直想着搭建一个自己的网站，这次终于付诸行动了。参考了网上的许多资料，所以在这里就简单记录下自己的建站过程。

###购买域名
从godaddy购买域名。也可用选择从万网和新网购买域名，这些网站还提供一条龙的服务，如提供主机，帮助备案等。

###选择托管网站的主机
购买虚拟主机或者VPS，也可选择存储在github上，我选择的后者（以后有时间再买个主机试试）。如果要将网页放在github上，需要申请一个github账号，并创建一个与帐号同名的项目。具体参考 [GitHub Pages](https://help.github.com/categories/github-pages-basics/)。 

###生成网页
Github推荐使用Jekyll构建自己的github pages。由于偏好python，我选择的pelican（鹈鹕）。Pelican是一个python包，使用pelican生成博客框架的过程如下：

    sudo apt-get install python-pip
    pip install pelican
    pip install markdown
    mkdir myblog
    cd myblog
    pelican-quickstart

在myblog/content目录下编写markdown文件：test.md,然后在myblog目录下执行如下命令：

    make html
    make serve
在myblog/output目录下会生成页面，同时在浏览器输入*localhost:8000*可以查看结果。


Markdown语法可参考 [Markdown Basics](https://help.github.com/articles/markdown-basics/)，Ubuntu-14.04下貌似默认安装了编辑器retext。

此外，需要加入如下几个功能：

1. 选择博客主题。下载pelican主题项目进行配置。

        git clone --recursive https://github.com/getpelican/pelican-themes.git
        cd pelican-themes
        pelican-themes -i ./pelican-themes/elegent

2. 增加sitemap。下载pelican插件项目进行配置。

        git clone https://github.com/getpelican/pelican-plugins
        cd pelican-plguins

3. 增加评论。[disqus](https://disqus.com/)提供了评论功能，注册账号即可获取一个shortname，将shortname加入pelicanconf.py,生成的页面中就会加入评论功能。

4. 增加站长统计。可选择google或者百度的站长工具。如果选择gogole，在 [Google Analytics](http://www.google.com/analytics) 创建帐号，将追踪ID加入pelicanconf.py，生成页面中就会加入追踪功能。在 [Google Webmasters](http://www.google.com/webmasters) 可查看追踪结果。

最后，配置文件myblog/pelicanconf.py基本如下：
    
    AUTHOR = u'lostfish'
    SITENAME = u"lostfish"
    SITEURL = 'http://tangke.me'
    THEME = 'elegent'
    DISQUS_SITENAME = 'disqus提供的shortname'
    GOOGLE_ANALYTICS = 'google提供的追踪id'
     
    PATH = 'content' #网页内容对应的markdown文件路径
    ...
    # 设置生成页面存储路径为pots/年/月/日/{slug}.html (slug即文章标题拼音)
    USE_FOLDER_AS_CATEGORY = False
    ARTICLE_URL = 'posts/{date:%Y}/{date:%m}/{date:%d}/{slug}.html'
    ARTICLE_SAVE_AS = 'posts/{date:%Y}/{date:%m}/{date:%d}/{slug}.html'
    PAGE_URL = 'pages/{slug}.html'
    PAGE_SAVE_AS = 'pages/{slug}.html'
    YEAR_ARCHIVE_SAVE_AS = 'posts/{date:%Y}/index.html'
    MONTH_ARCHIVE_SAVE_AS = 'posts/{date:%Y}/{date:%m}/index.html'

    #站点地图插件配置
    PLUGIN_PATHS = ["pelican-plugins"]
    PLUGINS = ['sitemap', 'extract_toc', 'tipue_search', 'liquid_tags.img',
                'neighbors', 'latex', 'related_posts', 'share_post']
    SITEMAP = {
        'format': 'xml',
        'priorities': {
            'articles': 0.7,
            'indexes': 0.5,
            'pages': 0.3,
        },
        'changefreqs': {
            'articles': 'monthly',
            'indexes': 'daily',
            'pages': 'monthly'
        }
    }    

###绑定域名
将自己购买的域名与github page绑定在一起需要3步：

1. 在github pages项目下增加文件CNAME，在该文件中加入购买的域名。
2. 在godaddy中将购买域名对应的域名解析服务器绑定到dnspod的服务器，可参考: [Godaddy注册商域名修改DNS地址](https://support.dnspod.cn/Kb/showarticle/?qtype=%E5%8A%9F%E8%83%BD%E4%BB%8B%E7%BB%8D%E5%8F%8A%E4%BD%BF%E7%94%A8%E6%95%99%E7%A8%8B&tsid=42)。
3. 注册dnspod账户，将自己购买的域名与github的服务器绑定,建立两条A类记指向192.30.252.153和192.30.252.154（这个IP可能会变化，参考github上 [教程](https://help.github.com/articles/tips-for-configuring-an-a-record-with-your-dns-provider/)）。另外，这个配置最晚可能要等72小时之后才生效。  

###有用参考
* [Pelican官方文档](http://docs.getpelican.com/en/3.5.0/settings.html)
* [Pelican主题列表](http://www.pelicanthemes.com/)
* [主题elegent配置说明](http://oncrashreboot.com/elegant-best-pelican-theme-features)
