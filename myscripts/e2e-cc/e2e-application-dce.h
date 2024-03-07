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

#ifndef E2E_APPLICATION_DCE_H
#define E2E_APPLICATION_DCE_H

#include "ns3/e2e-component.h"
#include "ns3/e2e-probe.h"
#include "ns3/e2e-host.h"

#include "ns3/network-module.h"
#include "ns3/dce-module.h"

namespace ns3
{

/* ... */

class E2EDceApplication : public E2EComponent
{
  public:
    E2EDceApplication(const E2EConfig& config);

    static Ptr<E2EDceApplication> CreateDceApplication(const E2EConfig& config);

    Ptr<Application> GetApplication();
    void AddToHost(Ptr<E2EHost> host);

  protected:
    DceApplicationHelper m_dceHelper;
    const E2EConfigValue* m_startTime;
    const E2EConfigValue* m_stopTime;
    Ptr<Application> m_application;

};

} // namespace ns3

#endif /* E2E_APPLICATION_DCE_H */