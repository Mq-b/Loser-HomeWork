import markdownit from 'markdown-it'
import { execSync } from 'child_process';
import fs from 'fs';
import tmp from 'tmp';
import mdkatex from 'markdown-it-katexx'

const markdown = markdownit({
    html: true,
    breaks: true,
    linkify: true,
    typographer: true,
    highlight: function (str, lang) {
        const highlightCode = (str, lang) => {
            const tempFile = tmp.fileSync(lang ? { postfix: `.${lang}` } : {});
            fs.writeFileSync(tempFile.name, str);

            try {
                const output = execSync(`tree-sitter highlight -H ${tempFile.name}`).toString();
                return output;
            } catch (error) {
                console.error('Error highlighting code:', error);
                return '';
            } finally {
                tempFile.removeCallback();
            }
        };

        return highlightCode(str, lang);
        //return '';
    }
});

markdown.use(mdkatex);

async function read(stream) {
    const chunks = [];
    for await (const chunk of stream) chunks.push(chunk);
    return Buffer.concat(chunks).toString('utf8');
}

const md = await read(process.stdin)
console.log(markdown.render(md))
