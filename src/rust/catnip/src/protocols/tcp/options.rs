// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.
use std::time::Duration;
use crate::protocols::tcp::constants::{DEFAULT_MSS, MIN_MSS, MAX_MSS};


#[derive(Clone, Debug)]
pub struct TcpOptions {
    pub advertised_mss: usize,
    pub handshake_retries: usize,
    pub handshake_timeout: Duration,
    pub receive_window_size: usize,
    pub retries: usize,
    pub trailing_ack_delay: Duration,
}

impl Default for TcpOptions {
    fn default() -> Self {
        TcpOptions {
            advertised_mss: DEFAULT_MSS,
            handshake_retries: 5,
            handshake_timeout: Duration::from_secs(3),
            receive_window_size: 0xffff,
            retries: 5,
            trailing_ack_delay: Duration::from_micros(1),
        }
    }
}

impl TcpOptions {
    pub fn advertised_mss(mut self, value: usize) -> Self {
        assert!(value >= MIN_MSS);
        assert!(value <= MAX_MSS);
        self.advertised_mss = value;
        self
    }

    pub fn handshake_retries(mut self, value: usize) -> Self {
        assert!(value > 0);
        self.handshake_retries = value;
        self
    }

    pub fn handshake_timeout(mut self, value: Duration) -> Self {
        assert!(value > Duration::new(0, 0));
        self.handshake_timeout = value;
        self
    }

    pub fn receive_window_size(mut self, value: usize) -> Self {
        assert!(value > 0);
        self.receive_window_size = value;
        self
    }

    pub fn retries(mut self, value: usize) -> Self {
        assert!(value > 0);
        self.retries = value;
        self
    }

    pub fn trailing_ack_delay(mut self, value: Duration) -> Self {
        self.trailing_ack_delay = value;
        self
    }
}
