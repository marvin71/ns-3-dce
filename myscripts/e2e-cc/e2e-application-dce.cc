/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

/*
 * Copyright 2023 Max Planck Institute for Software Systems
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "e2e-application-dce.h"

#include "ns3/applications-module.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("E2EDceApplication");

E2EDceApplication::E2EDceApplication(const E2EConfig& config)
    : E2EComponent(config)
{
    NS_ABORT_MSG_IF(GetId().empty(), "DCE application has no id");
    NS_ABORT_MSG_IF(GetIdPath().size() != 3,
        "DCE application '" << GetId() << "' has invalid path length of " << GetIdPath().size());

    m_startTime = config.Find("StartTime");
    m_stopTime = config.Find("StopTime");

    auto binary = config.Find("Binary");
    NS_ABORT_MSG_UNLESS(binary, "No binary given for DCE application " << GetId());
    m_dceHelper.SetBinary(std::string(*binary));

    if (auto stackSize {config.Find("StackSize")}; stackSize)
    {
        m_dceHelper.SetStackSize(E2EConfig::ConvertArgToUInteger(std::string(*stackSize)));
    }
    else
    {
        // if no stack size given, set it to 1 MiB
        m_dceHelper.SetStackSize(1 << 20);
    }

    m_dceHelper.ResetArguments();
    m_dceHelper.ResetEnvironment();
    if (auto arguments {config.Find("Arguments")}; arguments)
    {
        m_dceHelper.ParseArguments(std::string(*arguments));
    }
    if (auto environment {config.Find("Environment")}; environment)
    {
        // parse environment: KEY1=VALUE1,KEY2=VALUE2,...
        std::string_view env = *environment;
        size_t pos = 0;
        while (pos < env.size())
        {
            auto del = env.find('=', pos);
            NS_ABORT_MSG_IF(del == std::string_view::npos,
                "invalid environment variable, missing '='");
            std::string key {env.substr(pos, del - pos)};
            pos = del + 1;
            del = env.find(',', pos);
            std::string value;
            if (del == std::string_view::npos)
            {
                value = std::string(env.substr(pos, env.size() - pos));
                pos = env.size();
            }
            else
            {
                value = std::string(env.substr(pos, del - pos));
                pos = del + 1;
            }
            m_dceHelper.AddEnvironment(key, value);
        }
    }

    if (auto stdinFile {config.Find("StdinFile")}; stdinFile)
    {
        m_dceHelper.SetStdinFile(std::string(*stdinFile));
    }
}

Ptr<E2EDceApplication>
E2EDceApplication::CreateDceApplication(const E2EConfig& config)
{
    return Create<E2EDceApplication>(config);
}

Ptr<Application>
E2EDceApplication::GetApplication()
{
    return m_application;
}

void
E2EDceApplication::AddToHost(Ptr<E2EHost> host) {
    Ptr<Node> node = host->GetNode();
    NS_ABORT_MSG_UNLESS(node,
        "Trying to install DCE application " << GetId() << " on host with no node");
    DceManagerHelper dceManager;
    dceManager.Install(node);

    m_application = m_dceHelper.InstallInNode(node).Get(0);
    if (m_startTime)
    {
        m_application->SetStartTime(Time(std::string(*m_startTime)));
    }
    if (m_stopTime)
    {
        m_application->SetStopTime(Time(std::string(*m_stopTime)));
    }
    host->AddE2EComponent(this);
}

} // namespace ns3