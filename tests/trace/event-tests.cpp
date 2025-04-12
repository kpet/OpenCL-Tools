// Copyright 2024 The OpenCL-Tools authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "testcl.hpp"
#include <gtest/gtest.h>

TEST_F(WithCommandQueue, clCreateUserEventTest) {
    auto event = CreateUserEvent();
}

TEST_F(WithContext, clSetUserEventStatusTest) {
    auto event = CreateUserEvent();
    SetUserEventStatus(event, CL_COMPLETE);
}

TEST_F(WithContext, clWaitForEventsTest) {
    auto event1 = CreateUserEvent();
    auto event2 = CreateUserEvent();

    cl_event events[2] = {event1, event2};

    SetUserEventStatus(event1, CL_COMPLETE);
    SetUserEventStatus(event2, CL_COMPLETE);

    clWaitForEvents(2, &events[0]);
}

#if ENABLE_UNIMPLEMENTED
TEST_F(WithCommandQueue, clEnqueueWaitForEventsTest) {
    auto event1 = CreateUserEvent();
    auto event2 = CreateUserEvent();

    cl_event events[2] = {event1, event2};

    SetUserEventStatus(event1, CL_COMPLETE);
    SetUserEventStatus(event2, CL_COMPLETE);

    clEnqueueWaitForEvents(m_queue, 2, &events[0]);

    Finish();
}
#endif

#if ENABLE_UNIMPLEMENTED
struct event_user_data {};

void CL_CALLBACK event_callback_func(cl_event event,
                                     cl_int event_command_status,
                                     void* user_data) {}

TEST_F(WithCommandQueue, clSetEventCallbackTest) {
    auto event = CreateUserEvent();

    event_user_data user_data;

    cl_int err = clSetEventCallback(event, CL_COMPLETE, event_callback_func,
                                    static_cast<void*>(&user_data));
    ASSERT_CL_SUCCESS(err);
    SetUserEventStatus(event, CL_COMPLETE);

    // TODO: Consider testing testing callback with semaphore
}
#endif

#if ENABLE_UNIMPLEMENTED
TEST_F(WithCommandQueue, clEnqueueBarrierTest) {
    cl_int err = clEnqueueBarrier(m_queue);
    ASSERT_CL_SUCCESS(err);
}
#endif

#if ENABLE_UNIMPLEMENTED
TEST_F(WithCommandQueue, clEnqueueMarkerTest) {
    cl_event event;
    cl_int err = clEnqueueMarker(m_queue, &event);
    ASSERT_CL_SUCCESS(err);
    Finish();

    err = clReleaseEvent(event);
    ASSERT_CL_SUCCESS(err);
}
#endif

#if ENABLE_UNIMPLEMENTED
TEST_F(WithCommandQueue, clEnqueueBarrierWithWaitListTest) {
    auto event1 = CreateUserEvent();
    auto event2 = CreateUserEvent();
    SetUserEventStatus(event1, CL_COMPLETE);
    SetUserEventStatus(event2, CL_COMPLETE);

    cl_event events[] = {event1, event2};
    cl_int err = clEnqueueBarrierWithWaitList(m_queue, 2, &events[0], nullptr);
    ASSERT_CL_SUCCESS(err);

    Finish();
}
#endif

#if ENABLE_UNIMPLEMENTED
TEST_F(WithCommandQueue, clEnqueueMarkerWithWaitListTest) {
    auto event1 = CreateUserEvent();
    auto event2 = CreateUserEvent();
    SetUserEventStatus(event1, CL_COMPLETE);
    SetUserEventStatus(event2, CL_COMPLETE);

    cl_event events[] = {event1, event2};
    cl_int err = clEnqueueMarkerWithWaitList(m_queue, 2, &events[0], nullptr);
    ASSERT_CL_SUCCESS(err);

    Finish();
}
#endif