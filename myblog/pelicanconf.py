#!/usr/bin/env python
# -*- coding: utf-8 -*- #
from __future__ import unicode_literals

AUTHOR = u'lostfish'
SITENAME = u"当你老了"
SITEURL = ''
#SITEURL = 'http://tangke.me'
DISQUS_SITENAME = u'lostfish'
GOOGLE_ANALYTICS = u'UA-58905600-1'

PATH = 'content'

TIMEZONE = 'Asia/Chongqing'

DEFAULT_LANG = u'en'

# Feed generation is usually not desired when developing
FEED_ALL_ATOM = None
CATEGORY_FEED_ATOM = None
TRANSLATION_FEED_ATOM = None
AUTHOR_FEED_ATOM = None
AUTHOR_FEED_RSS = None

# Theme elegant configuration
TYPOGRIFY = True
DEFAULT_PAGINATION = False
THEME = 'pelican-themes/elegant' #'subtle'
LANDING_PAGE_ABOUT = {'title':"Jimmy Tang's Homepage", 'details': "My name is Jimmy Tang. I'm a programmer from Beijing, China. My job is about data mining and text analysis. In my daily work, I mainly use c/c++ and python. The work is full of challenge and I'm still on the road...<br>I like listening to the songs of singer Faye Wong, Sodagreen, Tizzy Bac, Eason Chan, Joanna Wang and so on. I love to reading some books about history, autobiograph and literature. I really like Russel's writings which is full of widsom. The best words he said is 'Diversity is essential to happiness'."}
MD_EXTENSIONS = ['codehilite(css_class=highlight)', 'extra', 'headerid', 'toc']
RECENT_ARTICLES_COUNT = 10
COMMENTS_INTRO = u"欢迎评论:"
SOCIAL_PROFILE_LABEL = u'Stay in Touch'
RELATED_POSTS_LABEL = 'Keep Reading'
SHARE_POST_INTRO = 'Like this post? Share on:'
SITE_LICENSE = ""
#LINKS = (('Pelican', 'http://getpelican.com/'),
#         ('Python.org', 'http://python.org/'),)
SOCIAL = (
        ('weibo', 'http://www.weibo.com/u/2272983007'),
        ('twitter', 'http://twitter.com/SeptWinds'),
        )

# Uncomment following line if you want document-relative URLs when developing
#RELATIVE_URLS = True

STATIC_PATHS = ['images', 'extra/CNAME', 'theme/images']
EXTRA_PATH_METADATA = {'extra/CNAME': {'path': 'CNAME'},}
DIRECT_TEMPLATES = (('index', 'tags', 'categories','archives', 'search', '404'))
TAG_SAVE_AS = ''
AUTHOR_SAVE_AS = ''
CATEGORY_SAVE_AS = ''
USE_SHORTCUT_ICONS = True

USE_FOLDER_AS_CATEGORY = False
ARTICLE_URL = 'posts/{date:%Y}/{date:%m}/{date:%d}/{slug}.html'
ARTICLE_SAVE_AS = 'posts/{date:%Y}/{date:%m}/{date:%d}/{slug}.html'
PAGE_URL = 'pages/{slug}.html'
PAGE_SAVE_AS = 'pages/{slug}.html'
YEAR_ARCHIVE_SAVE_AS = 'posts/{date:%Y}/index.html'
MONTH_ARCHIVE_SAVE_AS = 'posts/{date:%Y}/{date:%m}/index.html'

#DEFAULT_DATE_FORMAT = '%Y-%m-%d'
#LOCALE = 'zh_CN'
DATE_FORMATS = {
        'en': '%a, %d %b %Y',
        'zh': '%Y-%m-%d(%a)',
}

# Use pelican plugns
PLUGIN_PATHS = ["pelican-plugins"]
#PLUGINS = ["sitemap", "extract_toc", "tipue_search"]
PLUGINS = ['sitemap', 'extract_toc', 'tipue_search', 'liquid_tags.img',
            'neighbors', 'latex', 'related_posts', 'share_post']
SITEMAP = {
    "format": "xml",
    "priorities": {
        "articles": 0.7,
        "indexes": 0.5,
        "pages": 0.3,
    },
    "changefreqs": {
        "articles": "monthly",
        "indexes": "daily",
        "pages": "monthly",
    }
}

# Mailchimp
#EMAIL_SUBSCRIPTION_LABEL = u'Get Monthly Updates'
#EMAIL_FIELD_PLACEHOLDER = u'Enter your email...'
#SUBSCRIBE_BUTTON_TITLE = u'Send me Free updates'
#MAILCHIMP_FORM_ACTION = u'empty'

# Legal
SITE_LICENSE = u'<span xmlns:dct="http://purl.org/dc/terms/" property="dct:title">当你老了</span> by <a xmlns:cc="http://creativecommons.org/ns#" href="http://tangke.me" property="cc:attributionName" rel="cc:attributionURL">JimmyTang</a> is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-sa/3.0/deed.en_US">Creative Commons Attribution-ShareAlike 3.0 Unported License</a>.'

# SEO
SITE_DESCRIPTION = u'My name is Jimmy Tang \u2015 a software engineer who gets things done. This is my personal blog.'
