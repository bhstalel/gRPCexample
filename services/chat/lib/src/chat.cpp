#include "toolboxServiceApi.h"

Status ToolboxServiceImpl::ToolboxCommand(
    ServerContext* context,
    const ToolboxQuery* tbQuery,
    ServerWriter<ToolboxResult>* toolResultWriter){

        /**
         * Request will include :
         * - command_toolShape(a, b)
         * - interface type
         * - is_ipv6 active or not
         * - list of hosts 
         * 
         * Need to get interface type and get NetworkInterface with its info
         * check for command_tool and contruct command
         * check if (ipv6) concat -6 to command
         * check for regex with host and if its okay concat target host and execute command
        */

            Tins::NetworkInterface requestedInterface;
            try{
                if( tbQuery->iface() == sense::ToolboxQuery_Interface_ETHERNET ){
                    /* ETH0 */
                    spdlog::debug(" ETHERNET interface is selected. ");
                    requestedInterface = Tins::NetworkInterface(ethiface);
                }else if ( tbQuery->iface() == sense::ToolboxQuery_Interface_LTE4G ){
                    /* WWAN0 */
                    spdlog::debug(" 4G interface is selected. ");
                    requestedInterface = Tins::NetworkInterface(lteiface);
                }
            }catch(const std::exception& e){
                spdlog::error("Exception here :::: {}",e.what());
                return Status(grpc::StatusCode::CANCELLED, e.what());
            }

            if(tbQuery->command_tool() != sense::ToolboxQuery_Tools::ToolboxQuery_Tools_NETSTAT){
                /* Check interface state */
                std::string interface_file_path = "/sys/class/net/"+requestedInterface.name()+"/operstate" ;
                std::ifstream interface_file(interface_file_path);
                char content[50];
                while(interface_file.good()){
                    interface_file>>content;
                    if(interface_file.good() && strcmp(content, "down") == 0){
                        spdlog::error(" Interface : {} is down!! ", requestedInterface.name());
                        return Status(grpc::StatusCode::UNAVAILABLE, "Interface "+requestedInterface.name()+"is down!");
                    }
                }
            }

            /* Get ipv6 status */
            bool ipv6 = tbQuery->use_ipv6();
            if(ipv6==true){
                spdlog::debug(" IPV6 is selected. ");
                /* Check if hosts are IPV4 */
                for(int i=0; i<tbQuery->hosts_size(); i++) {
                    struct sockaddr_in sa;
                    if(inet_pton(AF_INET, tbQuery->hosts()[i].c_str(), &(sa.sin_addr))!=0){
                        spdlog::error(" Cannot use IPv4 hosts [{}] when enabling IPv6. ", tbQuery->hosts()[i]);
                        return Status(grpc::StatusCode::FAILED_PRECONDITION, "Cannot use IPv4 host ["+tbQuery->hosts()[i]+"] when enabling IPv6!");
                    }
                }
            }else spdlog::debug(" IPV6 is not selected. ");

        /* Get the command_tool name and set the corresponding command string */
        std::string cmd;
        switch (tbQuery->command_tool())
        {
            case sense::ToolboxQuery_Tools::ToolboxQuery_Tools_PING:
                cmd = "ping -c 5 -I " + requestedInterface.name();
                spdlog::debug(" Generating command for ping : " + cmd);
                break;
            case sense::ToolboxQuery_Tools::ToolboxQuery_Tools_TRACEROUTE:
                cmd = "traceroute -m 5 -i " + requestedInterface.name();
                spdlog::debug(" Generating command for traceroute : " + cmd);
                break;
            case sense::ToolboxQuery_Tools::ToolboxQuery_Tools_IPERF:
                cmd = "iperf3 -t 10 -B " + requestedInterface.ipv4_address().to_string() + " -c";
                spdlog::debug(" Generating command for iperf : " + cmd);
                break;
            case sense::ToolboxQuery_Tools::ToolboxQuery_Tools_NETSTAT:
                /* 
                    -n, --numeric            don't resolve names
                    --numeric-hosts          don't resolve host names
                    --numeric-ports          don't resolve port names
                    --numeric-users          don't resolve user names
                    -a, --all, --listening   display all sockets (default: connected)
                */
                cmd = "netstat -a -n --numeric-hosts --numeric-ports --numeric-users ";
                spdlog::debug(" Generating command for netstat : " + cmd);
                break;

            default:
                spdlog::error("Wrong command type");
                return Status(StatusCode::FAILED_PRECONDITION, "Wrong command type!");
                break;
        }

        /* Show arrived hosts list; */
        if(tbQuery->hosts().size() > 0){
            for(int i=0; i<tbQuery->hosts_size(); i++) {
                spdlog::debug(" Got host : " + tbQuery->hosts()[i]);
            }
        }

        /* Run command for all except NETSTAT */
        if( tbQuery->hosts().size() > 0 && tbQuery->command_tool() != sense::ToolboxQuery_Tools::ToolboxQuery_Tools_NETSTAT){
            for(int i=0; i<tbQuery->hosts_size(); i++) {
                GeneralCommand command(ipv6, requestedInterface, tbQuery->hosts()[i], cmd, toolResultWriter, tbQuery->command_tool());
                if(command.ExecuteCommand()==false){
                    return Status(StatusCode::INTERNAL, "Error executing command ["+cmd+"]");
                }
            }
        /* Executing NETSTAT */
        }else if( tbQuery->command_tool() == sense::ToolboxQuery_Tools::ToolboxQuery_Tools_NETSTAT ){
            spdlog::debug(" Running command Netstat ... ");
            GeneralCommand command(ipv6, requestedInterface, "", cmd, toolResultWriter, tbQuery->command_tool());
            if(command.ExecuteCommand()==false){
                return Status(StatusCode::INTERNAL, "Error executing command ["+cmd+"]");
            }
        }

        return Status::OK;
}
