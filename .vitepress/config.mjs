import { defineConfig } from 'vitepress';
import { repo_base, repo_url, repo_name, icon_url } from './theme/params';
import footnote_plugin from 'markdown-it-footnote';

const guildline_path = '/src/C++CoreGuidelines/';
const daily_path = '/src/卢瑟日经/';
const basic_path = '/src/基础性C++题目/'

// https://vitepress.dev/reference/site-config
export default defineConfig({
  themeConfig: {
    // https://vitepress.dev/reference/default-theme-config
    sidebar: [
      {
        items: [
          { text: '作业与解析', link: '/' },
          {
            text: 'C++CoreGuidelines',
            link: guildline_path + 'README.md',
            collapsed: true,
            items: [
              { text: '第一章-简介', link: guildline_path + '第1章-简介' },
              { text: '第二章-理念', link: guildline_path + '第2章-理念' },
              { text: '第三章-接口', link: guildline_path + '第3章-接口' },
              { text: '第四章-函数', link: guildline_path + '第4章-函数' },
              { text: '第五章-类和类层次结构', link: guildline_path + '第5章-类和类层次结构' },
            ]
          },
          {
            text: '卢瑟日经',
            link: daily_path + 'README.md',
            collapsed: true,
            items: [
              { text: '赋值运算符求值顺序的问题', link: daily_path + '赋值运算符求值顺序问题' },
              { text: '函数调用禁止隐式转换', link: daily_path + '函数调用禁止隐式转换' },
              { text: '数组&指针', link: daily_path + '数组&指针' },
              { text: '同步、异步、阻塞', link: daily_path + '同步、异步、阻塞' },
              { text: '为什么优先成员初始化器', link: daily_path + '为什么优先成员初始化器' },
              { text: '重载operator=要不要检查自赋值？', link: daily_path + '重载operator=要不要检查自赋值？' },
              { text: 'C&C++伪规则和错误的看法', link: daily_path + 'C&C++伪规则和错误的看法' },
              { text: 'catch(auto)的问题', link: daily_path + 'catch(auto)的问题' },
              { text: 'detach的问题', link: daily_path + 'detach的问题' },
              { text: '如何阅读STL源码？', link: daily_path + '如何阅读STL源码？' },
              { text: 'godbolt使用文档', link: daily_path + 'godbolt使用文档' },
            ]
          },
          {
            text: '基础性C++题目与答案',
            link: basic_path + 'README.md',
            collapsed: true,
            items: [
              { text: '20231002基础C++题目', link: basic_path + '20231002基础C++题目' },
              { text: '20231017基础C++题目（转换的使用）', link: basic_path + '20231017基础C++题目（转换的使用）' },
              { text: '20231126基础C++题目（数组退化）', link: basic_path + '20231126基础C++题目（数组退化）' },
            ]
          },
        ]
      },
    ],
    logo: icon_url,
    editLink: {
      pattern: repo_url + 'blob/main/:path',
      text: '在 GitHub 上编辑此页面'
    },
    socialLinks: [{ icon: 'github', link: repo_url }],
    search: {
      provider: 'local',
      options: {
        detailedView: true,
      }
    },
    outline: { label: '页面导航' },
    docFooter: {
      prev: '上一页',
      next: '下一页'
    },
    returnToTopLabel: '回到顶端',
  },

  rewrites: { 'README.md': 'index.md' },
  base: repo_base + '/',
  title: repo_name,
  head: [['link', { rel: 'icon', href: repo_base + icon_url }]],
  cleanUrls: true,
  ignoreDeadLinks: true,
  markdown: {
    config(md) {
      md.use(footnote_plugin);
    }
  },
})
