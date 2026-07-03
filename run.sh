#!/bin/bash

GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

echo -e "${BLUE}🔨 Building CAI with Intent...${NC}"
make

if [ $? -eq 0 ]; then
    echo -e "${GREEN}✅ Build successful!${NC}"
    echo -e "${YELLOW}🚀 Starting CAI...${NC}"
    echo ""
    ./bin/cai
else
    echo -e "${RED}❌ Build failed!${NC}"
    exit 1
fi
