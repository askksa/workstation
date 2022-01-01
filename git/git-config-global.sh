#!/bin/sh
template_dir=$HOME/.git-templates
tempalte_hooks_dir=$template_dir/hooks
mkdir -p $tempalte_hooks_dir
cp ./git-commit-template.txt $template_dir
cp ./pre-commit $tempalte_hooks_dir
chmod -R a+x $tempalte_hooks_dir

git config --global user.email "wenshuai.xi@sigmastar.com.cn"
git config --global user.name "wenshuai.xi"
git config --global merge.tool vimdiff
git config --global diff.tool vimdiff
git config --global core.editor vim
git config --global init.templatedir $template_dir
git config --global commit.template ~/.git-templates/git-commit-template.txt
