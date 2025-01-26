sudo apt update
wget -qO - https://packages.lunarg.com/lunarg-signing-key-pub.asc | sudo apt-key add -
sudo wget -qO /etc/apt/sources.list.d/lunarg-vulkan-1.3.283-focal.list https://packages.lunarg.com/vulkan/1.3.283/lunarg-vulkan-1.3.283-focal.list
sudo apt update
sudo apt install vulkan-sdk
sudo apt install vulkan-tools vulkan-validationlayers-dev spirv-tools
sudo apt install vulkan-tools -y
sudo apt install libvulkan-dev -y
sudo apt install vulkan-validationlayers-dev spirv-tools -y