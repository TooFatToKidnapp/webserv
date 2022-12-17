#!/bin/bash

find . -name ".DS_Store" -print -delete
rm -f a.out
make fclean
git add .
git checkout
echo "Enter your message to commit:"
read message
git commit -m "$message"
git push
