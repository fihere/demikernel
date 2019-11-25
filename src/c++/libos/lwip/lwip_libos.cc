// -*- mode: c++; c-file-style: "k&r"; c-basic-offset: 4 -*-

// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#include "lwip_queue.hh"

#include <dmtr/annot.h>
#include <dmtr/libos.h>
#include <dmtr/libos/memory_queue.hh>
#include <dmtr/libos/io_queue_api.hh>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <rte_mbuf.h>

#include <memory>

static std::unique_ptr<dmtr::io_queue_api> ioq_api;

int dmtr_init(int argc, char *argv[])
{
    DMTR_NULL(EINVAL, ioq_api.get());

    DMTR_OK(dmtr::lwip_queue::init_dpdk(argc, argv));

    dmtr::io_queue_api *p = NULL;
    DMTR_OK(dmtr::io_queue_api::init(p, argc, argv));
    ioq_api = std::unique_ptr<dmtr::io_queue_api>(p);
    ioq_api->register_queue_ctor(dmtr::io_queue::MEMORY_Q, dmtr::memory_queue::new_object);
    ioq_api->register_queue_ctor(dmtr::io_queue::NETWORK_Q, dmtr::lwip_queue::new_object);
    ioq_api->register_queue_ctor(dmtr::io_queue::SHARED_Q, dmtr::shared_queue::new_object);
    return 0;
}

int dmtr_queue(int *qd_out)
{
    DMTR_NOTNULL(EINVAL, qd_out);
    DMTR_NOTNULL(EINVAL, ioq_api.get());

    DMTR_OK(ioq_api->queue(*qd_out));
    return 0;
}

int dmtr_socket(int *qd_out, int domain, int type, int protocol)
{
    DMTR_NOTNULL(EINVAL, qd_out);
    DMTR_NOTNULL(EINVAL, ioq_api.get());

    return ioq_api->socket(*qd_out, domain, type, protocol);
}

int dmtr_getsockname(int qd, struct sockaddr * const saddr, socklen_t * const size)
{
    DMTR_NOTNULL(EPERM, ioq_api.get());

    return ioq_api->getsockname(qd, saddr, size);
}

int dmtr_listen(int qd, int backlog)
{
    DMTR_NOTNULL(EINVAL, ioq_api.get());

    return ioq_api->listen(qd, backlog);
}

int dmtr_bind(int qd, const struct sockaddr * const saddr, socklen_t size)
{
    DMTR_NOTNULL(EINVAL, ioq_api.get());

    return ioq_api->bind(qd, saddr, size);
}

int dmtr_accept(dmtr_qtoken_t *qtok_out, int sockqd)
{
    DMTR_NOTNULL(EINVAL, qtok_out);
    DMTR_NOTNULL(EPERM, ioq_api.get());

    return ioq_api->accept(*qtok_out, sockqd);
}

int dmtr_connect(int qd, const struct sockaddr *saddr, socklen_t size)
{
    DMTR_NOTNULL(EINVAL, ioq_api.get());

    return ioq_api->connect(qd, saddr, size);
}

int dmtr_close(int qd)
{
    DMTR_NOTNULL(EINVAL, ioq_api.get());

    return ioq_api->close(qd);
}

int dmtr_is_qd_valid(int *flag_out, int qd)
{
    DMTR_NOTNULL(EINVAL, flag_out);
    *flag_out = 0;
    DMTR_NOTNULL(EPERM, ioq_api.get());

    bool b = false;
    DMTR_OK(ioq_api->is_qd_valid(b, qd));
    if (b) {
        *flag_out = 1;
    }

    return 0;
}

int dmtr_push(dmtr_qtoken_t *qtok_out, int qd, const dmtr_sgarray_t *sga)
{
    DMTR_NOTNULL(EINVAL, qtok_out);
    DMTR_NOTNULL(EINVAL, sga);
    DMTR_NOTNULL(EINVAL, ioq_api.get());

    return ioq_api->push(*qtok_out, qd, *sga);
}

int dmtr_pop(dmtr_qtoken_t *qtok_out, int qd)
{
    DMTR_NOTNULL(EINVAL, qtok_out);
    DMTR_NOTNULL(EINVAL, ioq_api.get());

    return ioq_api->pop(*qtok_out, qd);
}

int dmtr_poll(dmtr_qresult_t *qr_out, dmtr_qtoken_t qt)
{
    DMTR_NOTNULL(EINVAL, ioq_api.get());

    return ioq_api->poll(qr_out, qt);
}

int dmtr_drop(dmtr_qtoken_t qt)
{
    DMTR_NOTNULL(EINVAL, ioq_api.get());

    return ioq_api->drop(qt);
}

int dmtr_open2(int *qd_out, const char *pathname, int flags, mode_t mode) {
    DMTR_NOTNULL(EINVAL, qd_out);
    DMTR_NOTNULL(EPERM, ioq_api.get());

    return 0;
}

int dmtr_free_mbuf(dmtr_sgarray_t *sga) {
    DMTR_NOTNULL(EINVAL, sga);

    if (sga->mbuf == NULL) {
        free(sga->sga_buf);
    } else {
        struct rte_mbuf *mbuf = static_cast<struct rte_mbuf *>(sga->mbuf);
        rte_pktmbuf_free(mbuf);
    }

    return 0;
}
